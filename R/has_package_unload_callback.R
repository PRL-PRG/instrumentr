
#' @export
has_package_unload_callback <- function(object, ...) {
    UseMethod("has_package_unload_callback")
}

#' @export
has_package_unload_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_unload_callback, object)
}
