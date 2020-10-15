
#' @export
has_closure_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_closure_call_exit_callback")
}

#' @export
has_closure_call_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_closure_call_exit, object)
}
