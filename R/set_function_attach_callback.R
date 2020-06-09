
#' @export
set_function_attach_callback <- function(context, function_attach_callback, ...) {
    UseMethod("set_function_attach_callback")
}

#' @export
set_function_attach_callback.lightr_context <- function(context, function_attach_callback, ...) { # nolint

    stopifnot(is_undefined(function_attach_callback) ||
              is_closure(function_attach_callback) &&
              length(formals(function_attach_callback)) >= 4)

    .Call(C_context_set_function_attach_callback,
          context,
          function_attach_callback)

    invisible(NULL)
}
