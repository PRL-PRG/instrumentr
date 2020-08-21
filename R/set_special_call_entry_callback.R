
#' @export
set_special_call_entry_callback <- function(context, callback, ...) { # nolint
    UseMethod("set_special_call_entry_callback")
}

#' @export
set_special_call_entry_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_special_call_entry_callback(callback)) {
        callback <- create_special_call_entry_callback(callback)
    }

    .Call(C_context_set_special_call_entry_callback, context, callback)

    invisible(NULL)
}
