
#' @export
set_package_unload_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_unload_callback")
}

#' @export
set_package_unload_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_unload_callback(callback)) {
        callback <- create_package_unload_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_unload, tracer, callback)

    invisible(NULL)
}
