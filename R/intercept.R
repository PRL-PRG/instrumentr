remove_interception <- function() {

    ## NOTE: directly calling unlockBinding results in
    ## warnings on R CMD check
    local_unlock_binding <- get("unlockBinding", envir=baseenv())

    unintercept_function <- function(intercepted_function) {
        package_name <- intercepted_function$package_name
        package_env <- intercepted_function$package_env
        function_name <- intercepted_function$function_name
        new_function_obj <- intercepted_function$function_obj
        old_function_obj <- intercepted_function$old_function_obj

        local_unlock_binding(function_name, package_env)
        assign(function_name, old_function_obj, envir = package_env)
        lockBinding(function_name, package_env)
    }

    for(function_id in get_intercepted_function_ids()) {
        unintercept_function(get_intercepted_function(function_id))
        remove_intercepted_function(function_id)
    }
}

insert_interception <- function(context_ptr, application_ptr) {

    handle_package <- function(package_name, ...) {
        ##autoinject_packages <- get_autoinject()
        ##if (package_name %in% autoinject_packages || any(autoinject_packages
        ##== "all")) {

        package_env <- getNamespace(package_name)

        package_dir <- dirname(system.file(package=package_name))

        package_ptr <- create_package(package_name, package_dir, package_env)

        add_package(application_ptr, package_ptr)

        .Call(C_lightr_trace_package_entry, context_ptr, application_ptr, package_ptr)

        tryCatch({

            package <- intercept_package(context_ptr, application_ptr, package_ptr)

            .Call(C_lightr_trace_package_exit, context_ptr, application_ptr, package_ptr)

        }, error = function(e) {

            message(e$message)

        })
    }

    with_tracing_disabled({

        traced_packages <- get_traced_packages(context_ptr)

        remove_packages <- c(".GlobalEnv", "Autoloads", "tools:callr", "tools:rstudio", "lightr")

        loaded_packages <- setdiff(remove_package_prefix(search()), remove_packages)

        for(package in intersect(traced_packages, loaded_packages)) {
            handle_package(package)
        }

        for (package in setdiff(traced_packages, loaded_packages)) {
            setHook(packageEvent(package, "attach"), handle_package)
        }

    })
}

intercept_package <- function(context_ptr, application_ptr, package_ptr) {

    package_name <- get_name(package_ptr)

    package_env <- get_environment(package_ptr)

    all_function_names <- ls(envir=package_env, all.names = TRUE)

    traced_function_names <- get_traced_functions(context_ptr, package_name)

    if(length(traced_function_names) == 0) {
        function_names <- all_function_names
    }
    else {
        function_names <- intersect(traced_function_names, all_function_names)
    }

    function_ptrs <- list()

    for (function_name in function_names) {

        function_obj <- get(function_name, envir=package_env)

        if(!is_closure(function_obj)) next

        function_ptr <- create_function(function_name, length(formals(function_obj)), function_obj)

        add_function(package_ptr, function_ptr)

        .Call(C_lightr_trace_function_entry, context_ptr, application_ptr, package_ptr, function_ptr)

        tryCatch({

            package <- intercept_function(context_ptr, application_ptr, package_ptr, function_ptr)

            .Call(C_lightr_trace_function_exit, context_ptr, application_ptr, package_ptr, function_ptr)

        }, error = function(e) {

            message <- sprintf("unable to intercept `%s::%s`: %s",
                               package_name, function_name, e$message)

            message(e$message)

        })
    }

    message("Intercepting ", length(get_functions(package_ptr)), " functions from ", package_name)
}

#' @importFrom injectr sexp_address
is_intercepted <- function(fun) {
    id <- sexp_address(fun)
    has_intercepted_function(id)
}

#' @importFrom injectr sexp_address
intercept_function <- function(context_ptr, application_ptr, package_ptr, function_ptr) {

    package_name <- get_name(package_ptr)
    package_env <- get_environment(package_ptr)

    function_name <- get_name(function_ptr)
    function_obj <- get_definition(function_ptr)

    if (is_intercepted(function_obj)) {
        msg <- sprintf("'%s::%s' already intercepted", package_name, function_name)
        message(msg)
    }
    else {
        function_id <- sexp_address(function_obj)

        old_function_obj <- modify_function(context_ptr, application_ptr, package_ptr, function_ptr)

        add_intercepted_function(function_id, list(package_name=package_name,
                                                   package_env=package_env,
                                                   function_name=function_name,
                                                   new_function_obj=function_obj,
                                                   old_function_obj=old_function_obj))
    }
}

#' @importFrom injectr inject_code create_duplicate
modify_function <- function(context_ptr, application_ptr, package_ptr, function_ptr) {

    function_obj <- get_definition(function_ptr)

    old_function_obj <- create_duplicate(function_obj)

    check_params <- create_argval_tracing_code(context_ptr, application_ptr, package_ptr, function_ptr)
    inject_code(check_params, function_obj)

    check_retval <- create_retval_tracing_code(context_ptr, application_ptr, package_ptr, function_ptr)
    inject_code(check_retval, function_obj, where="onexit")

    old_function_obj
}

create_argval_tracing_code <- function(context_ptr, application_ptr, package_ptr, function_ptr) {
    substitute({
        if(.Call(IS_TRACING_ENABLED)) {
            .Call(DISABLE_TRACING)
            .Call(TRACE_CALL_ENTRY,
                  CONTEXT_PTR,
                  APPLICATION_PTR,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  create_call(FUNCTION_PTR, sys.call(), environment()))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_lightr_is_tracing_enabled,
            DISABLE_TRACING=C_lightr_disable_tracing,
            TRACE_CALL_ENTRY=C_lightr_trace_call_entry,
            CONTEXT_PTR=context_ptr,
            APPLICATION_PTR=application_ptr,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr,
            REINSTATE_TRACING=C_lightr_reinstate_tracing))
}


create_retval_tracing_code <- function(context_ptr, application_ptr, package_ptr, function_ptr) {
    substitute({
        if(.Call(IS_TRACING_ENABLED)) {
            .Call(DISABLE_TRACING)
            .Call(TRACE_CALL_EXIT,
                  CONTEXT_PTR,
                  APPLICATION_PTR,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  returnValue(INVALID_VALUE))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_lightr_is_tracing_enabled,
            DISABLE_TRACING=C_lightr_disable_tracing,
            TRACE_CALL_EXIT=C_lightr_trace_call_exit,
            CONTEXT_PTR=context_ptr,
            APPLICATION_PTR=application_ptr,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr,
            INVALID_VALUE=invalid_value,
            REINSTATE_TRACING=C_lightr_reinstate_tracing))
}
