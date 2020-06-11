
#' @export
set_function_detach_callback <- function(context, function_detach_callback, ...) {
    UseMethod("set_function_detach_callback")
}

#' @export
set_function_detach_callback.instrumentr_context <- function(context, function_detach_callback, ...) {  # nolint

    stopifnot(is_undefined(function_detach_callback) ||
              is_closure(function_detach_callback) &&
              has_parameters(function_detach_callback, 4))

    .Call(C_context_set_function_detach_callback,
          context,
          function_detach_callback)

    invisible(NULL)
}
