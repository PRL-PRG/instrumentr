
set_call_entry_callback <- function(context, call_entry_callback, ...) {
    UseMethod("set_call_entry_callback")
}

set_call_entry_callback.lightr_context <- function(context, call_entry_callback, ...) {  # nolint

    stopifnot(is_invalid_value(call_entry_callback) ||
              is_closure(call_entry_callback) &&
              length(formals(call_entry_callback)) >= 5)

    assign("call_entry_callback", call_entry_callback, envir=get_environment(context))

    .Call(C_context_set_call_entry_callback,
          context,
          call_entry_callback)

    invisible(NULL)
}
