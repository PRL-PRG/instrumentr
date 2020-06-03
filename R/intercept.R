intercept_package <- function(package_name) {

    package_env <- getNamespace(package_name)

    package_dir <- dirname(system.file(package=package_name))

    package_ptr <- create_package(package_name, package_dir, package_env)

    .Call(C_lightr_intercept_package_entry, package_ptr)

    function_ptrs <- intercept_environment(package_ptr, package_name, package_env, all.names = TRUE)

    for(function_ptr in function_ptrs) {
        if(!is.null(function_ptr)) {
            add_function(package_ptr, function_ptr)
        }
    }

    application <- get_application()

    add_package(application, package_ptr)

    packageStartupMessage("Intercepting ", length(get_functions(package_ptr)), " functions from ", package_name)

    package_ptr
}

intercept_environment <- function(package_ptr, package_name, package_env, ...) {
    ## TODO: check for package_ptr type
    stopifnot(is_environment(package_env))
    stopifnot(is_scalar_character(package_name))

    function_names <- ls(envir=package_env, ...)

    function_ptrs <- list()

    for (function_name in function_names) {

        function_obj <- get(function_name, envir=package_env)

        if(!is_closure(function_obj)) next

        unlockBinding(function_name, package_env)
        assign(function_name, function_obj, envir = package_env)
        lockBinding(function_name, package_env)

        tryCatch({
            function_ptr <- intercept_function(package_ptr, package_name, package_env, function_name, function_obj)
            function_ptrs <- c(function_ptrs, list(function_ptr))

        }, error = function(e) {
            message <- sprintf("unable to intercept `%s::%s`: %s",
                               package_name, function_name, e$message)
            print(message)
        })
    }

    function_ptrs
}

#' @importFrom injectr sexp_address
is_intercepted <- function(fun) {
    id <- sexp_address(fun)
    has_intercepted_function(id)
}

#' @importFrom injectr sexp_address
intercept_function <- function(package_ptr, package_name, package_env, function_name, function_obj) {
    ## TODO: check type of package_ptr
    stopifnot(is_function(function_obj))
    stopifnot(is_scalar_character(function_name))
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_environment(package_env))

    function_ptr <- NULL

    if (is_intercepted(function_obj)) {
        msg <- sprintf("'%s::%s' already intercepted", package_name, function_name)
        packageStartupMessage(msg)
    }
    else {
        function_ptr <- create_function(function_name, length(formals(function_obj)), function_obj)
        function_id <- sexp_address(function_obj)
        old_function_obj <- modify_function(package_ptr, package_name, package_env, function_ptr, function_name, function_obj)
        add_intercepted_function(function_id, list(package_name=package_name,
                                                   package_env=package_env,
                                                   function_name=function_name,
                                                   new_function_obj=function_obj,
                                                   old_function_obj=old_function_obj))
    }

    function_ptr
}

#' @importFrom injectr inject_code create_duplicate
modify_function <- function(package_ptr, package_name, package_env, function_ptr, function_name, function_obj) {

    old_function_obj <- create_duplicate(function_obj)

    check_params <- create_argval_interception_code(package_ptr, package_name, package_env, function_ptr, function_name, function_obj)
    inject_code(check_params, function_obj)

    check_retval <- create_retval_interception_code(package_ptr, package_name, package_env, function_ptr, function_name, function_obj)
    inject_code(check_retval, function_obj, where="onexit")

    old_function_obj
}

create_argval_interception_code <- function(package_ptr, package_name, package_env, function_ptr, function_name, function_obj) {
    substitute({
        if(.Call(IS_TRACING_ENABLED)) {
            .Call(DISABLE_TRACING)
            .Call(INTERCEPT_CALL_ENTRY,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  sys.function(),
                  environment(),
                  sys.frame(sys.nframe()))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_lightr_is_tracing_enabled,
            REINSTATE_TRACING=C_lightr_reinstate_tracing,
            DISABLE_TRACING=C_lightr_disable_tracing,
            INTERCEPT_CALL_ENTRY=C_lightr_intercept_call_entry,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr))
}


create_retval_interception_code <- function(package_ptr, package_name, package_env, function_ptr, function_name, function_obj) {
    substitute({
        if(.Call(IS_TRACING_ENABLED)) {
            .Call(DISABLE_TRACING)
            .Call(INTERCEPT_CALL_EXIT,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  returnValue(NO_RETVAL_MARKER),
                  !identical(returnValue(NO_RETVAL_MARKER), NO_RETVAL_MARKER))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_lightr_is_tracing_enabled,
            REINSTATE_TRACING=C_lightr_reinstate_tracing,
            DISABLE_TRACING=C_lightr_disable_tracing,
            INTERCEPT_CALL_EXIT=C_lightr_intercept_call_exit,
            NO_RETVAL_MARKER=.no_retval_marker,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr))
}
