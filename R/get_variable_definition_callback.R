
#' @export
get_variable_definition_callback <- function(context, ...) { # nolint
    UseMethod("get_variable_definition_callback")
}

#' @export
get_variable_definition_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_definition_callback, context)
}
