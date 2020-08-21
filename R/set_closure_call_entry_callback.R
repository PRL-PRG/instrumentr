
#' @export
set_closure_call_entry_callback <- function(context, callback, ...) { # nolint
    UseMethod("set_closure_call_entry_callback")
}

#' @export
set_closure_call_entry_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_closure_call_entry_callback(callback)) {
        callback <- create_closure_call_entry_callback(callback)
    }

    .Call(C_context_set_closure_call_entry_callback, context, callback)

    invisible(NULL)
}
