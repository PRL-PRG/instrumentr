
#' @export
get_call_entry_callback <- function(context, ...) {
    UseMethod("get_call_entry_callback")
}

#' @export
get_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_call_entry_callback, context)
}
