
#' @export
set_application_attach_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_application_attach_callback")
}

#' @export
set_application_attach_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_application_attach_callback(callback)) {
        callback <- create_application_attach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_application_attach, tracer, callback)

    invisible(NULL)
}
