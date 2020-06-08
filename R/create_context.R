
#' @export
create_context <- function(application_load_callback,
                           application_unload_callback,
                           application_attach_callback,
                           application_detach_callback,
                           package_load_callback,
                           package_unload_callback,
                           package_attach_callback,
                           package_detach_callback,
                           function_attach_callback,
                           function_detach_callback,
                           call_entry_callback,
                           call_exit_callback,
                           packages = character(0),
                           functions = character(0)) {

    if (missing(application_load_callback)) {
        application_load_callback <- invalid_value
    }

    if (missing(application_unload_callback)) {
        application_unload_callback <- invalid_value
    }

    if (missing(application_attach_callback)) {
        application_attach_callback <- invalid_value
    }

    if (missing(application_detach_callback)) {
        application_detach_callback <- invalid_value
    }

    if (missing(package_load_callback)) {
        package_load_callback <- invalid_value
    }

    if (missing(package_unload_callback)) {
        package_unload_callback <- invalid_value
    }

    if (missing(package_attach_callback)) {
        package_attach_callback <- invalid_value
    }

    if (missing(package_detach_callback)) {
        package_detach_callback <- invalid_value
    }

    if (missing(function_attach_callback)) {
        function_attach_callback <- invalid_value
    }

    if (missing(function_detach_callback)) {
        function_detach_callback <- invalid_value
    }

    if (missing(call_entry_callback)) {
        call_entry_callback <- invalid_value
    }

    if (missing(call_exit_callback)) {
        call_exit_callback <- invalid_value
    }

    stopifnot(is_vector_character(packages))

    stopifnot(is_vector_character(functions))

    environment <- new.env(parent=sys.frame(-1))

    context <- .Call(C_context_create_context, environment)

    set_application_load_callback(context, application_load_callback)
    set_application_unload_callback(context, application_unload_callback)
    set_application_attach_callback(context, application_attach_callback)
    set_application_detach_callback(context, application_detach_callback)

    set_package_load_callback(context, package_load_callback)
    set_package_unload_callback(context, package_unload_callback)
    set_package_attach_callback(context, package_attach_callback)
    set_package_detach_callback(context, package_detach_callback)

    set_function_attach_callback(context, function_attach_callback)
    set_function_detach_callback(context, function_detach_callback)

    set_call_entry_callback(context, call_entry_callback)
    set_call_exit_callback(context, call_exit_callback)

    trace_packages(context, packages)
    trace_functions(context, functions)

    context
}
