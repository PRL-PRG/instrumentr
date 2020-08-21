
#' @export
set_builtin_call_entry_callback <- function(context, callback, ...) { # nolint
    UseMethod("set_builtin_call_entry_callback")
}

#' @export
set_builtin_call_entry_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_builtin_call_entry_callback(callback)) {
        callback <- create_builtin_call_entry_callback(callback)
    }

    .Call(C_context_set_builtin_call_entry_callback, context, callback)

    invisible(NULL)
}
