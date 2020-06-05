
#' @export
create_context <- function(application_entry_callback,
                           application_exit_callback,
                           package_entry_callback,
                           package_exit_callback,
                           function_entry_callback,
                           function_exit_callback,
                           call_entry_callback,
                           call_exit_callback,
                           packages = character(0),
                           functions = character(0)) {

    if(missing(application_entry_callback)) {
        application_entry_callback <- invalid_value
    }

    if(missing(application_exit_callback)) {
        application_exit_callback <- invalid_value
    }

    if(missing(package_entry_callback)) {
        package_entry_callback <- invalid_value
    }

    if(missing(package_exit_callback)) {
        package_exit_callback <- invalid_value
    }

    if(missing(function_entry_callback)) {
        function_entry_callback <- invalid_value
    }

    if(missing(function_exit_callback)) {
        function_exit_callback <- invalid_value
    }

    if(missing(call_entry_callback)) {
        call_entry_callback <- invalid_value
    }

    if(missing(call_exit_callback)) {
        call_exit_callback <- invalid_value
    }

    stopifnot(is_invalid_value(application_entry_callback) ||
              is_closure(application_entry_callback) &&
              length(formals(application_entry_callback)) >= 2)

    stopifnot(is_invalid_value(application_exit_callback) ||
              is_closure(application_exit_callback) &&
              length(formals(application_exit_callback)) >= 2)

    stopifnot(is_invalid_value(package_entry_callback) ||
              is_closure(package_entry_callback) &&
              length(formals(package_entry_callback)) >= 3)

    stopifnot(is_invalid_value(package_exit_callback) ||
              is_closure(package_exit_callback) &&
              length(formals(package_exit_callback)) >= 3)

    stopifnot(is_invalid_value(function_entry_callback) ||
              is_closure(function_entry_callback) &&
              length(formals(function_entry_callback)) >= 4)

    stopifnot(is_invalid_value(function_exit_callback) ||
              is_closure(function_exit_callback) &&
              length(formals(function_exit_callback)) >= 4)

    stopifnot(is_invalid_value(call_entry_callback) ||
              is_closure(call_entry_callback) &&
              length(formals(call_entry_callback)) >= 5)

    stopifnot(is_invalid_value(call_exit_callback) ||
              is_closure(call_exit_callback) &&
              length(formals(call_exit_callback)) >= 5)

    stopifnot(is_vector_character(packages))

    stopifnot(is_vector_character(functions))

    environment <- sys.frame(sys.nframe())

    context <- .Call(C_context_create_context,
                     application_entry_callback,
                     application_exit_callback,
                     package_entry_callback,
                     package_exit_callback,
                     function_entry_callback,
                     function_exit_callback,
                     call_entry_callback,
                     call_exit_callback,
                     environment)

    trace_functions(context, functions)

    trace_packages(context, packages)

    context
}
