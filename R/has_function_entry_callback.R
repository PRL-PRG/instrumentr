
#' @export
has_function_entry_callback <- function(object, ...) {
    UseMethod("has_function_entry_callback")
}

#' @export
has_function_entry_callback.lightr_context <- function(object, ...) { # nolint
    .Call(C_context_has_function_entry_callback, object)
}
