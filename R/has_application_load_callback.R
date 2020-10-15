
#' @export
has_application_load_callback <- function(object, ...) {
    UseMethod("has_application_load_callback")
}

#' @export
has_application_load_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_application_load, object)
}
