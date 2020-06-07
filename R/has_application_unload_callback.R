
#' @export
has_application_unload_callback <- function(object, ...) {
    UseMethod("has_application_unload_callback")
}

#' @export
has_application_unload_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_unload_callback, object)
}
