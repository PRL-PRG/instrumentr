
#' @export
set_application_detach_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_application_detach_callback")
}

#' @export
set_application_detach_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_application_detach_callback(callback)) {
        callback <- create_application_detach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_application_detach, tracer, callback)

    invisible(NULL)
}
