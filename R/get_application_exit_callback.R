
get_application_exit_callback <- function(context, ...) {
    UseMethod("get_application_exit_callback")
}

get_application_exit_callback.lightr_context <- function(context, ...) {
    .Call(C_context_get_application_exit_callback, context)
}
