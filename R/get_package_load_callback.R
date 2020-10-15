
#' @export
get_package_load_callback <- function(tracer, ...) {
    UseMethod("get_package_load_callback")
}

#' @export
get_package_load_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_load, tracer)
}
