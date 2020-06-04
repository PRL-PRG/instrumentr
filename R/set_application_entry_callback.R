
set_application_entry_callback <- function(context, application_entry_callback, ...) {
    UseMethod("set_application_entry_callback")
}

set_application_entry_callback.lightr_context <- function(context, application_entry_callback, ...) {
    stopifnot(is_closure(application_entry_callback))

    .Call(C_context_set_application_entry_callback, context, application_entry_callback)

    invisible(NULL)
}
