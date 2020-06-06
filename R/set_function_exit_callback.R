
#' @export
set_function_exit_callback <- function(context, function_exit_callback, ...) {
    UseMethod("set_function_exit_callback")
}

#' @export
set_function_exit_callback.lightr_context <- function(context, function_exit_callback, ...) {  # nolint

    stopifnot(is_null(function_exit_callback) ||
              is_closure(function_exit_callback) &&
              length(formals(function_exit_callback)) >= 4)

    .Call(C_context_set_function_exit_callback,
          context,
          function_exit_callback)

    invisible(NULL)
}
