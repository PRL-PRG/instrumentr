
#' @export
get_eval_exit_callback <- function(tracer, ...) {
    UseMethod("get_eval_exit_callback")
}

#' @export
get_eval_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_eval_exit, tracer)
}
