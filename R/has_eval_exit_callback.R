
#' @export
has_eval_exit_callback <- function(object, ...) {
    UseMethod("has_eval_exit_callback")
}

#' @export
has_eval_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_eval_exit, object)
}
