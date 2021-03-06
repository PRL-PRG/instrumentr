insert_instrumentation <- function(context_ptr, application_ptr) {

    handle_package <- function(package_name, ...) {
        package_env <- getNamespace(package_name)

        package_dir <- dirname(system.file(package=package_name))

        package_ptr <- create_package(package_name, package_dir, package_env)

        .Call(C_context_trace_package_load, context_ptr, application_ptr, package_ptr)

        instrument_package(context_ptr, application_ptr, package_ptr)

        .Call(C_context_trace_package_attach, context_ptr, application_ptr, package_ptr)

        ## remove hook after its job is done
        setHook(packageEvent(package_name, "onLoad"), NULL, "replace")
    }

    traced_packages <- get_traced_packages(context_ptr)

    remove_packages <- c("tools:callr", "tools:rstudio", "instrumentr")

    loaded_packages <- setdiff(loadedNamespaces(), remove_packages)

    for (package in intersect(traced_packages, loaded_packages)) {
        handle_package(package)
    }

    for (package in setdiff(traced_packages, loaded_packages)) {
        setHook(packageEvent(package, "onLoad"), handle_package)
    }
}

instrument_package <- function(context_ptr, application_ptr, package_ptr) {

    package_name <- get_name(package_ptr)

    package_env <- get_environment(package_ptr)

    function_table <- get_function_table(package_env)

    all_function_names <- ls(envir=package_env, all.names = TRUE)

    traced_function_names <- get_traced_functions(context_ptr, package_name)

    if (length(traced_function_names) == 0) {
        function_names <- all_function_names
    }
    else {
        function_names <- intersect(traced_function_names, all_function_names)
    }

    function_ptrs <- list()

    for (function_name in function_names) {

        function_obj <- get(function_name, envir=package_env)

        if (!is_closure(function_obj)) next

        function_entry <- get0(function_name,
                               envir=function_table,
                               ifnotfound=list(public=FALSE, s3_generic=FALSE, s3_method=FALSE))

        function_ptr <- create_function(function_name,
                                        length(formals(function_obj)),
                                        function_obj,
                                        function_entry$public,
                                        function_entry$s3_generic,
                                        function_entry$s3_method)

        .Call(C_context_trace_function_attach, context_ptr, application_ptr, package_ptr, function_ptr)

        package <- instrument_function(context_ptr, application_ptr, package_ptr, function_ptr)

    }

    message("Instrumented ", length(get_functions(package_ptr)), " functions from ", package_name)
}

is_instrumented <- function(package_name, function_name) {
    has_instrumented_function(package_name, function_name)
}

instrument_function <- function(context_ptr, application_ptr, package_ptr, function_ptr) {

    package_name <- get_name(package_ptr)
    package_env <- get_environment(package_ptr)

    function_name <- get_name(function_ptr)
    function_obj <- get_definition(function_ptr)

    if (is_instrumented(package_name, function_name)) {
        msg <- sprintf("'%s::%s' already instrumented", package_name, function_name)
        message(msg)
    }
    else {
        old_function_obj <- modify_function(context_ptr, application_ptr, package_ptr, function_ptr)

        add_instrumented_function(package_name,
                                  function_name,
                                  list(package_name=package_name,
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
        if (.Call(IS_TRACING_ENABLED)) {
            ## NOTE: disabling is needed to prevent recursion
            ## from calling environment and sys.call closures
            .Call(DISABLE_TRACING)
            .Call(TRACE_CALL_ENTRY,
                  CONTEXT_PTR,
                  APPLICATION_PTR,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  .Call(CREATE_CALL, FUNCTION_PTR, sys.call(), environment(), sys.nframe()))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_instrumentr_is_tracing_enabled,
            DISABLE_TRACING=C_instrumentr_disable_tracing,
            TRACE_CALL_ENTRY=C_context_trace_call_entry,
            CONTEXT_PTR=context_ptr,
            APPLICATION_PTR=application_ptr,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr,
            CREATE_CALL=C_call_create_call,
            REINSTATE_TRACING=C_instrumentr_reinstate_tracing))
}

create_retval_tracing_code <- function(context_ptr, application_ptr, package_ptr, function_ptr) {
    substitute({
        if (.Call(IS_TRACING_ENABLED)) {
            ##NOTE: disabling is needed to prevent recursion
            ##from calling returnValue closure
            .Call(DISABLE_TRACING)
            .Call(TRACE_CALL_EXIT,
                  CONTEXT_PTR,
                  APPLICATION_PTR,
                  PACKAGE_PTR,
                  FUNCTION_PTR,
                  returnValue(UNDEFINED_OBJECT))
            .Call(REINSTATE_TRACING)
        }
    }, list(IS_TRACING_ENABLED=C_instrumentr_is_tracing_enabled,
            DISABLE_TRACING=C_instrumentr_disable_tracing,
            TRACE_CALL_EXIT=C_context_trace_call_exit,
            CONTEXT_PTR=context_ptr,
            APPLICATION_PTR=application_ptr,
            PACKAGE_PTR=package_ptr,
            FUNCTION_PTR=function_ptr,
            UNDEFINED_OBJECT=undefined_object,
            REINSTATE_TRACING=C_instrumentr_reinstate_tracing))
}
