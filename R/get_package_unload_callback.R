
#' @export
get_package_unload_callback <- function(tracer, ...) {
    UseMethod("get_package_unload_callback")
}

#' @export
get_package_unload_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_unload, tracer)
}
