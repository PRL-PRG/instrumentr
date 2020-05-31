
set_call_entry_callback <- function(context, call_entry_callback, ...) {
    UseMethod("set_call_entry_callback")
}

set_call_entry_callback.lightr_context <- function(context, call_entry_callback, ...) {
    stopifnot(is_closure(call_entry_callback))

    .Call(C_context_set_call_entry_callback,
          context,
          call_entry_callback)

    invisible(NULL)
}
