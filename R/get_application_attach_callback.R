
#' @export
get_application_attach_callback <- function(tracer, ...) { # nolint
    UseMethod("get_application_attach_callback")
}

#' @export
get_application_attach_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_application_attach, tracer)
}
