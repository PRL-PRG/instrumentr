intercept_package <- function(package_name) {
    stopifnot(is_scalar_character(package_name))

    package_env <- getNamespace(package_name)

    intercept_environment(package_env, package_name, all.names = TRUE)
}

intercept_environment <- function(env, envname, ...) {
    stopifnot(is_environment(env))
    stopifnot(is_scalar_character(envname))

    var_names <- ls(envir=env, ...)

    modified_var_names <- character(0)

    for (var_name in var_names) {

        fun <- get(var_name, envir=env)
        if(!is_closure(fun)) next

        tryCatch({
            intercept_function(fun, var_name, envname,  env)
            modified_var_names <- c(modified_var_names, var_name)

        }, error = function(e) {
            message <- sprintf("unable to intercept `%s::%s`: %s",
                               envname, var_name, e$message)
            print(message)
        })
    }

    modified_var_names
}

is_intercepted <- function(fun) {
    id <- injectr:::sexp_address(fun)
    has_intercepted_function(id)
}

intercept_function <- function(fun, name, pkg, env) {
    stopifnot(is_function(fun))
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(pkg))
    stopifnot(is_environment(env))

    if (is_intercepted(fun)) {
        msg <- sprintf("'%s::%s' already intercepted", pkg, name)
        packageStartupMessage(msg)
    }
    else {
        id <- injectr:::sexp_address(fun)
        old <- modify_function(fun, name, pkg)
        add_intercepted_function(id, list(env = env,
                                          pkg=pkg,
                                          fun_name=name,
                                          new=fun,
                                          old=old))
    }

    invisible(NULL)
}

modify_function <- function(fun, fun_name, pkg) {
    old <- injectr:::create_duplicate(fun)

    check_params <- create_argval_interception_code(fun_name, pkg)
    injectr::inject_code(check_params, fun)

    check_retval <- create_retval_interception_code(fun_name, pkg)
    injectr::inject_code(check_retval, fun, where="onexit")

    old
}

create_argval_interception_code <- function(fun_name, pkg) {
    substitute({
        if(.Call(.lightr_interception_is_enabled)) {
            .Call(.lightr_disable_interception)
            .Call(.lightr_intercept_call_entry,
                  PKG,
                  FUN_NAME,
                  sys.function(),
                  sys.frame(sys.nframe()))
            .Call(.lightr_enable_interception)
        }
    }, list(PKG=pkg, FUN_NAME=fun_name))
}


create_retval_interception_code <- function(fun_name, pkg) {
    substitute({
        if(.Call(.lightr_interception_is_enabled)) {
            .Call(.lightr_disable_interception)
            .Call(.lightr_intercept_call_exit,
                  PKG,
                  FUN_NAME,
                  returnValue(.lightr_no_retval_marker),
                  !identical(returnValue(.lightr_no_retval_marker), .lightr_no_retval_marker))
            .Call(.lightr_enable_interception)
        }
    }, list(PKG=pkg, FUN_NAME=fun_name))
}
