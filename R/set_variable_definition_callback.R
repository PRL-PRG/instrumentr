
#' @export
set_variable_definition_callback <- function(tracer, callback, ...) { # nolint
    UseMethod("set_variable_definition_callback")
}

#' @export
set_variable_definition_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_variable_definition_callback(callback)) {
        callback <- create_variable_definition_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_definition, tracer, callback)

    invisible(NULL)
}
