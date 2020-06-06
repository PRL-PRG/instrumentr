
set_application_entry_callback <- function(context, application_entry_callback, ...) {
    UseMethod("set_application_entry_callback")
}

set_application_entry_callback.lightr_context <- function(context, application_entry_callback, ...) {  # nolint

    stopifnot(is_invalid_value(application_entry_callback) ||
              is_closure(application_entry_callback) &&
              length(formals(application_entry_callback)) >= 2)

    assign("application_entry_callback", application_entry_callback, envir=get_environment(context))

    .Call(C_context_set_application_entry_callback, context, application_entry_callback)

    invisible(NULL)
}
