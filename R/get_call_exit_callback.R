
#' @export
get_call_exit_callback <- function(context, ...) {
    UseMethod("get_call_exit_callback")
}

#' @export
get_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_call_exit_callback, context)
}
