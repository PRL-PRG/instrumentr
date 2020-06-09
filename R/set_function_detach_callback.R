
#' @export
set_function_detach_callback <- function(context, function_detach_callback, ...) {
    UseMethod("set_function_detach_callback")
}

#' @export
set_function_detach_callback.lightr_context <- function(context, function_detach_callback, ...) {  # nolint

    stopifnot(is_undefined(function_detach_callback) ||
              is_closure(function_detach_callback) &&
              length(formals(function_detach_callback)) >= 4)

    .Call(C_context_set_function_detach_callback,
          context,
          function_detach_callback)

    invisible(NULL)
}
