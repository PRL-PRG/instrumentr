
set_call_entry_callback <- function(context, call_entry_callback, ...) {
    UseMethod("set_call_entry_callback")
}

set_call_entry_callback.instrumentr_context <- function(context, call_entry_callback, ...) {  # nolint

    stopifnot(is_undefined(call_entry_callback) ||
              is_closure(call_entry_callback) &&
              has_parameters(call_entry_callback, 5))

    .Call(C_context_set_call_entry_callback,
          context,
          call_entry_callback)

    invisible(NULL)
}
