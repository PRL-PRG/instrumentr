
#' @export
set_package_detach_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_detach_callback")
}

#' @export
set_package_detach_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_detach_callback(callback)) {
        callback <- create_package_detach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_detach, tracer, callback)

    invisible(NULL)
}
