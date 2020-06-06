
#' @export
set_function_entry_callback <- function(context, function_entry_callback, ...) {
    UseMethod("set_function_entry_callback")
}

#' @export
set_function_entry_callback.lightr_context <- function(context, function_entry_callback, ...) { # nolint

    stopifnot(is_invalid_value(function_entry_callback) ||
              is_closure(function_entry_callback) &&
              length(formals(function_entry_callback)) >= 4)

    assign("function_entry_callback", function_entry_callback, envir=get_environment(context))

    .Call(C_context_set_function_entry_callback,
          context,
          function_entry_callback)

    invisible(NULL)
}
