
#' @export
set_closure_call_entry_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_closure_call_entry_callback")
}

#' @export
set_closure_call_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_closure_call_entry_callback(callback)) {
        callback <- create_closure_call_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_closure_call_entry, tracer, callback)

    invisible(NULL)
}
