
#' @export
get_closure_call_exit_callback <- function(context, ...) { # nolint
    UseMethod("get_closure_call_exit_callback")
}

#' @export
get_closure_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_closure_call_exit_callback, context)
}
