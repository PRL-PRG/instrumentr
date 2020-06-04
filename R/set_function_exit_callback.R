
#' @export
set_function_exit_callback <- function(context, function_exit_callback, ...) {
    UseMethod("set_function_exit_callback")
}

#' @export
set_function_exit_callback.lightr_context <- function(context, function_exit_callback, ...) {
    stopifnot(is_closure(function_exit_callback))

    .Call(C_context_set_function_exit_callback,
          context,
          function_exit_callback)

    invisible(NULL)
}
