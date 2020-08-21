
#' @export
get_builtin_call_exit_callback <- function(context, ...) { # nolint
    UseMethod("get_builtin_call_exit_callback")
}

#' @export
get_builtin_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_builtin_call_exit_callback, context)
}
