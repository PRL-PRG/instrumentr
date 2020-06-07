
#' @export
has_application_detach_callback <- function(object, ...) {
    UseMethod("has_application_detach_callback")
}

#' @export
has_application_detach_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_detach_callback, object)
}
