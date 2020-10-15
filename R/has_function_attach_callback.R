
#' @export
has_function_attach_callback <- function(object, ...) {
    UseMethod("has_function_attach_callback")
}

#' @export
has_function_attach_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_function_attach, object)
}
