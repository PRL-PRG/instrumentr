
#' @export
get_builtin_call_entry_callback <- function(tracer, ...) { # nolint
    UseMethod("get_builtin_call_entry_callback")
}

#' @export
get_builtin_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_builtin_call_entry, tracer)
}
