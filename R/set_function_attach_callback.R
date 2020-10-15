
#' @export
set_function_attach_callback <- function(tracer, callback, ...) {
    UseMethod("set_function_attach_callback")
}

#' @export
set_function_attach_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_function_attach_callback(callback)) {
        callback <- create_function_attach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_function_attach, tracer, callback)

    invisible(NULL)
}
