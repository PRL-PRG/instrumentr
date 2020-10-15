
#' @export
get_function_attach_callback <- function(tracer, ...) {
    UseMethod("get_function_attach_callback")
}

#' @export
get_function_attach_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_function_attach, tracer)
}
