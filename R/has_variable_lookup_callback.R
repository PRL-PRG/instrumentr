
#' @export
has_variable_lookup_callback <- function(object, ...) {
    UseMethod("has_variable_lookup_callback")
}

#' @export
has_variable_lookup_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_lookup, object)
}
