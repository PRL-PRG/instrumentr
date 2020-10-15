
#' @export
has_variable_definition_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_definition_callback")
}

#' @export
has_variable_definition_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_definition, object)
}
