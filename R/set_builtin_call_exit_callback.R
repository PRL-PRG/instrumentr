
#' @export
set_builtin_call_exit_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_builtin_call_exit_callback")
}

#' @export
set_builtin_call_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_builtin_call_exit_callback(callback)) {
        callback <- create_builtin_call_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_builtin_call_exit, tracer, callback)

    invisible(NULL)
}
