
#' @export
get_special_call_exit_callback <- function(tracer, ...) { # nolint
    UseMethod("get_special_call_exit_callback")
}

#' @export
get_special_call_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_special_call_exit, tracer)
}
