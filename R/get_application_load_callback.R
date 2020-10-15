
#' @export
get_application_load_callback <- function(tracer, ...) {
    UseMethod("get_application_load_callback")
}

#' @export
get_application_load_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_application_load, tracer)
}
