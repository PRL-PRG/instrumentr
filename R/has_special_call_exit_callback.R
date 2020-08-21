
#' @export
has_special_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_exit_callback")
}

#' @export
has_special_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_special_call_exit_callback, object)
}
