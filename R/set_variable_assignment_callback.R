
#' @export
set_variable_assignment_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_variable_assignment_callback")
}

#' @export
set_variable_assignment_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_variable_assignment_callback(callback)) {
        callback <- create_variable_assignment_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_assignment, tracer, callback)

    invisible(NULL)
}
