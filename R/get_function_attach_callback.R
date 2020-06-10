
#' @export
get_function_attach_callback <- function(context, ...) {
    UseMethod("get_function_attach_callback")
}

#' @export
get_function_attach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_function_attach_callback, context)
}
