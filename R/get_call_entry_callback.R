
#' @export
get_call_entry_callback <- function(tracer, ...) {
    UseMethod("get_call_entry_callback")
}

#' @export
get_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_call_entry, tracer)
}
