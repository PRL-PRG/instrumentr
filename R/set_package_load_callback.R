
#' @export
set_package_load_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_load_callback(callback)) {
        callback <- create_package_load_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_load, tracer, callback)

    invisible(NULL)
}
