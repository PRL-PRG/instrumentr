
#' @export
set_application_load_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_application_load_callback")
}

#' @export
set_application_load_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_application_load_callback(callback)) {
        callback <- create_application_load_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_application_load, tracer, callback)
    invisible(NULL)
}
