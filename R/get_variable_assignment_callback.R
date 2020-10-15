
#' @export
get_variable_assignment_callback <- function(tracer, ...) { # nolint
    UseMethod("get_variable_assignment_callback")
}

#' @export
get_variable_assignment_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_assignment, tracer)
}
