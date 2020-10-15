
#' @export
set_package_attach_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_attach_callback")
}

#' @export
set_package_attach_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_attach_callback(callback)) {
        callback <- create_package_attach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_attach, tracer, callback)

    invisible(NULL)
}
