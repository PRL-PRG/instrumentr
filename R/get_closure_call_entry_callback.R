
#' @export
get_closure_call_entry_callback <- function(tracer, ...) { # nolint
    UseMethod("get_closure_call_entry_callback")
}

#' @export
get_closure_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_closure_call_entry, tracer)
}
