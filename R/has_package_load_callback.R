
#' @export
has_package_load_callback <- function(object, ...) {
    UseMethod("has_package_load_callback")
}

#' @export
has_package_load_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_load_callback, object)
}
