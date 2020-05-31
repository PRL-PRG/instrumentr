
get_call_entry_callback <- function(context, ...) {
    UseMethod("get_call_entry_callback")
}

get_call_entry_callback.lightr_context <- function(context, ...) {
    .Call(C_context_get_call_entry_callback, context)
}
