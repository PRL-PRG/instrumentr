
#' @export
set_application_unload_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_application_unload_callback")
}

#' @export
set_application_unload_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_application_unload_callback(callback)) {
        callback <- create_application_unload_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_application_unload, tracer, callback)

    invisible(NULL)
}
