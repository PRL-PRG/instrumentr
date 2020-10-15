
#' @export
has_call_exit_callback <- function(object, ...) {
    UseMethod("has_call_exit_callback")
}

#' @export
has_call_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_call_exit, object)
}
