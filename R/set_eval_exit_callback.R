
#' @export
set_eval_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_eval_exit_callback")
}

#' @export
set_eval_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_eval_exit_callback(callback)) {
        callback <- create_eval_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_eval_exit, tracer, callback)

    invisible(NULL)
}
