
#' @export
get_variable_removal_callback <- function(tracer, ...) {
    UseMethod("get_variable_removal_callback")
}

#' @export
get_variable_removal_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_removal, tracer)
}
