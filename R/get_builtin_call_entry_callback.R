
#' @export
get_builtin_call_entry_callback <- function(context, ...) { # nolint
    UseMethod("get_builtin_call_entry_callback")
}

#' @export
get_builtin_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_builtin_call_entry_callback, context)
}
