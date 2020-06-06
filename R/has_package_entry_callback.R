
#' @export
has_package_entry_callback <- function(object, ...) {
    UseMethod("has_package_entry_callback")
}

#' @export
has_package_entry_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_entry_callback, object)
}
