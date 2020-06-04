
#' @export
create_context <- function(application_entry_callback = NULL,
                           application_exit_callback = NULL,
                           package_entry_callback = NULL,
                           package_exit_callback = NULL,
                           function_entry_callback = NULL,
                           function_exit_callback = NULL,
                           call_entry_callback = NULL,
                           call_exit_callback = NULL) {

    stopifnot(is_null(application_entry_callback) ||
              is_closure(application_entry_callback) &&
              length(formals(application_entry_callback)) >= 2)

    stopifnot(is_null(application_exit_callback) ||
              is_closure(application_exit_callback) &&
              length(formals(application_exit_callback)) >= 2)

    stopifnot(is_null(package_entry_callback) ||
              is_closure(package_entry_callback) &&
              length(formals(package_entry_callback)) >= 3)

    stopifnot(is_null(package_exit_callback) ||
              is_closure(package_exit_callback) &&
              length(formals(package_exit_callback)) >= 3)

    stopifnot(is_null(function_entry_callback) ||
              is_closure(function_entry_callback) &&
              length(formals(function_entry_callback)) >= 4)

    stopifnot(is_null(function_exit_callback) ||
              is_closure(function_exit_callback) &&
              length(formals(function_exit_callback)) >= 4)

    stopifnot(is_null(call_entry_callback) ||
              is_closure(call_entry_callback) &&
              length(formals(call_entry_callback)) >= 5)

    stopifnot(is_null(call_exit_callback) ||
              is_closure(call_exit_callback) &&
              length(formals(call_exit_callback)) >= 5)

    environment <- sys.frame(sys.nframe())

    .Call(C_context_create_context,
          application_entry_callback,
          application_exit_callback,
          package_entry_callback,
          package_exit_callback,
          function_entry_callback,
          function_exit_callback,
          call_entry_callback,
          call_exit_callback,
          environment)
}
