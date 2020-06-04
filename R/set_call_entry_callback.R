
set_call_entry_callback <- function(context, call_entry_callback, ...) {
    UseMethod("set_call_entry_callback")
}

set_call_entry_callback.lightr_context <- function(context, call_entry_callback, ...) {

    stopifnot(is_null(call_entry_callback) ||
              is_closure(call_entry_callback) &&
              length(formals(call_entry_callback)) >= 5)

    .Call(C_context_set_call_entry_callback,
          context,
          call_entry_callback)

    invisible(NULL)
}
