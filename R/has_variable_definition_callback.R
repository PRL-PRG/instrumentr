
#' @export
has_variable_definition_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_definition_callback")
}

#' @export
has_variable_definition_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_definition_callback, object)
}
