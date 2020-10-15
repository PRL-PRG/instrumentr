
#' @export
get_environment <- function(object, ...) {
    UseMethod("get_environment")
}

#' @export
get_environment.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_get_environment, object)
}

#' @export
get_environment.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_environment, object)
}

#' @export
get_environment.instrumentr_call <- function(object, ...) { # nolint
    .Call(C_instrumentr_call_get_environment, object)
}
