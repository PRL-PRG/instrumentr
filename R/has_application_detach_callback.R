
#' @export
has_application_detach_callback <- function(object, ...) { # nolint
    UseMethod("has_application_detach_callback")
}

#' @export
has_application_detach_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_application_detach, object)
}
