
#' @export
has_builtin_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_builtin_call_exit_callback")
}

#' @export
has_builtin_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_builtin_call_exit_callback, object)
}
