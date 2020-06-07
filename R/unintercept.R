
unintercept_function <- function(context_ptr, application_ptr, package_ptr, function_ptr) {

    package_name <- get_name(package_ptr)
    function_name <- get_name(function_ptr)
    intercepted_function <- get_intercepted_function(package_name, function_name)

    package_env <- intercepted_function$package_env
    new_function_obj <- intercepted_function$function_obj
    old_function_obj <- intercepted_function$old_function_obj

    ## NOTE: directly calling unlockBinding results in
    ## warnings on R CMD check
    local_unlock_binding <- get("unlockBinding", envir=baseenv())

    local_unlock_binding(function_name, package_env)
    assign(function_name, old_function_obj, envir = package_env)
    lockBinding(function_name, package_env)

    remove_intercepted_function(package_name, function_name)

    .Call(C_lightr_trace_function_exit, context_ptr, application_ptr, package_ptr, function_ptr)
}

unintercept_package <- function(context_ptr, application_ptr, package_ptr) {

    .Call(C_lightr_trace_package_detach, context_ptr, application_ptr, package_ptr)

    package_name <- get_name(package_ptr)
    function_ptrs <- rev(get_functions(package_ptr))

    for (function_ptr in function_ptrs) {
        unintercept_function(context_ptr, application_ptr, package_ptr, function_ptr)
    }

    .Call(C_lightr_trace_package_unload, context_ptr, application_ptr, package_ptr)

    remove_intercepted_package(package_name)
}

remove_interception <- function(context_ptr, application_ptr) {
    package_ptrs <- rev(get_packages(application_ptr))

    for (package_ptr in package_ptrs) {
        unintercept_package(context_ptr, application_ptr, package_ptr)
    }
}
