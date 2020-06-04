
#' @export
set_function_entry_callback <- function(context, function_entry_callback, ...) {
    UseMethod("set_function_entry_callback")
}

#' @export
set_function_entry_callback.lightr_context <- function(context, function_entry_callback, ...) {
    stopifnot(is_closure(function_entry_callback))

    .Call(C_context_set_function_entry_callback,
          context,
          function_entry_callback)

    invisible(NULL)
}

