
#' @export
has_package_exit_callback <- function(object, ...) {
    UseMethod("has_package_exit_callback")
}

#' @export
has_package_exit_callback.lightr_context <- function(object, ...) {
    .Call(C_context_has_package_exit_callback, object)
}
