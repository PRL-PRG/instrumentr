
#' @export
has_package_load_callback <- function(object, ...) {
    UseMethod("has_package_load_callback")
}

#' @export
has_package_load_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_load, object)
}
