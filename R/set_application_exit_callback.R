
set_application_exit_callback <- function(context, application_exit_callback, ...) {
    UseMethod("set_application_exit_callback")
}

set_application_exit_callback.lightr_context <- function(context, application_exit_callback, ...) {  # nolint

    stopifnot(is_invalid_value(application_exit_callback) ||
              is_closure(application_exit_callback) &&
              length(formals(application_exit_callback)) >= 2)

    assign("application_exit_callback", application_exit_callback, envir=get_environment(context))

    .Call(C_context_set_application_exit_callback, context, application_exit_callback)

    invisible(NULL)
}
