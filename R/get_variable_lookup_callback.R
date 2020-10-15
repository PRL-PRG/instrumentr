
#' @export
get_variable_lookup_callback <- function(tracer, ...) {
    UseMethod("get_variable_lookup_callback")
}

#' @export
get_variable_lookup_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_lookup, tracer)
}
