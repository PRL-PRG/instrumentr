
#' @export
set_call_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_call_exit_callback")
}

#' @export
set_call_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_call_exit_callback(callback)) {
        callback <- create_call_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_call_exit, tracer, callback)

    invisible(NULL)
}
