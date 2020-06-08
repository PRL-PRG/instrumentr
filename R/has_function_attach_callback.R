
#' @export
has_function_attach_callback <- function(object, ...) {
    UseMethod("has_function_attach_callback")
}

#' @export
has_function_attach_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_function_attach_callback, object)
}
