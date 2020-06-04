
#' @export
get_function_exit_callback <- function(context, ...) {
    UseMethod("get_function_exit_callback")
}

#' @export
get_function_exit_callback.lightr_context <- function(context, ...) {
    .Call(C_context_get_function_exit_callback, context)
}
