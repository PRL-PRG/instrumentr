
#' @export
has_application_exit_callback <- function(object, ...) {
    UseMethod("has_application_exit_callback")
}

#' @export
has_application_exit_callback.lightr_context <- function(object, ...) {
    .Call(C_context_has_application_exit_callback, object)
}
