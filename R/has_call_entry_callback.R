
#' @export
has_call_entry_callback <- function(object, ...) {
    UseMethod("has_call_entry_callback")
}

#' @export
has_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_call_entry_callback, object)
}
