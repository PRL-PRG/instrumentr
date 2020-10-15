
#' @export
get_application_unload_callback <- function(tracer, ...) { # nolint
    UseMethod("get_application_unload_callback")
}

#' @export
get_application_unload_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_application_unload, tracer)
}
