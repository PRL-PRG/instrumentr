
#' @export
set_call_entry_callback <- function(context, callback, ...) {
    UseMethod("set_call_entry_callback")
}

#' @export
set_call_entry_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_call_entry_callback(callback)) {
        callback <- create_call_entry_callback(callback)
    }

    .Call(C_context_set_call_entry_callback, context, callback)

    invisible(NULL)
}
