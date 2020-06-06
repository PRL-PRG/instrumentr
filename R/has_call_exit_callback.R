
#' @export
has_call_exit_callback <- function(object, ...) {
    UseMethod("has_call_exit_callback")
}

#' @export
has_call_exit_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_call_exit_callback, object)
}
