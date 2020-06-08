
#' @export
get_function_attach_callback <- function(context, ...) {
    UseMethod("get_function_attach_callback")
}

#' @export
get_function_attach_callback.lightr_context <- function(context, ...) { # nolint
    .Call(C_context_get_function_attach_callback, context)
}
