
#' @export
create_variable_definition_callback <- function(object) { # nolint
    UseMethod("create_variable_definition_callback")
}

#' @export
create_variable_definition_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_variable_definition_create_from_r_function, object)
}

#' @export
create_variable_definition_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_definition_create_from_c_function, object)
}
