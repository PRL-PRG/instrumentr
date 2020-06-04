
#' @export
set_function_entry_callback <- function(context, function_entry_callback, ...) {
    UseMethod("set_function_entry_callback")
}

#' @export
set_function_entry_callback.lightr_context <- function(context, function_entry_callback, ...) {

    stopifnot(is_null(function_entry_callback) ||
              is_closure(function_entry_callback) &&
              length(formals(function_entry_callback)) >= 4)

    .Call(C_context_set_function_entry_callback,
          context,
          function_entry_callback)

    invisible(NULL)
}

