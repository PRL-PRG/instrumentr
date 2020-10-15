
#' @export
has_variable_assignment_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_assignment_callback")
}

#' @export
has_variable_assignment_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_assignment, object)
}
