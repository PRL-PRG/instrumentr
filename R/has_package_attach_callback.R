
#' @export
has_package_attach_callback <- function(object, ...) {
    UseMethod("has_package_attach_callback")
}

#' @export
has_package_attach_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_attach, object)
}
