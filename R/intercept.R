intercept_package <- function(package_name) {
    stopifnot(is_scalar_character(package_name))

    package_env <- getNamespace(package_name)

    package <- create_package(package_name, package_env)

    functions <- intercept_environment(package, package_env, package_name, all.names = TRUE)

    for(func in functions) {
        if(!is.null(func)) {
            add_function(package, func)
        }
    }

    package
}

intercept_environment <- function(package, env, name, ...) {
    stopifnot(is_environment(env))
    stopifnot(is_scalar_character(name))

    variables <- ls(envir=env, ...)

    functions <- list()

    for (variable in variables) {

        fun <- get(variable, envir=env)
        if(!is_closure(fun)) next

        tryCatch({
            func <- intercept_function(package, fun, variable, name, env)
            functions <- c(functions, list(func))

        }, error = function(e) {
            message <- sprintf("unable to intercept `%s::%s`: %s",
                               name, variable, e$message)
            print(message)
        })
    }

    functions
}

is_intercepted <- function(fun) {
    id <- injectr:::sexp_address(fun)
    has_intercepted_function(id)
}

intercept_function <- function(package, fun, name, pkg, env) {
    stopifnot(is_function(fun))
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(pkg))
    stopifnot(is_environment(env))

    func <- NULL

    if (is_intercepted(fun)) {
        msg <- sprintf("'%s::%s' already intercepted", pkg, name)
        packageStartupMessage(msg)
    }
    else {
        func <- create_function(name, length(formals(fun)))
        id <- injectr:::sexp_address(fun)
        old <- modify_function(package, func, fun)
        add_intercepted_function(id, list(env = env,
                                          pkg=pkg,
                                          fun_name=name,
                                          new=fun,
                                          old=old))
    }

    func
}

modify_function <- function(package, func, fun) {
    old <- injectr:::create_duplicate(fun)

    check_params <- create_argval_interception_code(package, func)
    injectr::inject_code(check_params, fun)

    check_retval <- create_retval_interception_code(package, func)
    injectr::inject_code(check_retval, fun, where="onexit")

    old
}

create_argval_interception_code <- function(package, func) {
    substitute({
        if(.Call(.lightr_interception_is_enabled)) {
            .Call(.lightr_disable_interception)
            .Call(.lightr_intercept_call_entry,
                  PKG,
                  FUNC,
                  sys.function(),
                  sys.frame(sys.nframe()))
            .Call(.lightr_enable_interception)
        }
    }, list(PKG=package, FUNC=func))
}


create_retval_interception_code <- function(package, func) {
    substitute({
        if(.Call(.lightr_interception_is_enabled)) {
            .Call(.lightr_disable_interception)
            .Call(.lightr_intercept_call_exit,
                  PKG,
                  FUNC,
                  returnValue(.lightr_no_retval_marker),
                  !identical(returnValue(.lightr_no_retval_marker), .lightr_no_retval_marker))
            .Call(.lightr_enable_interception)
        }
    }, list(PKG=package, FUNC=func))
}
