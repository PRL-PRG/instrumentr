
#' @export
create_variable_assignment_callback <- function(object) { # nolint
    UseMethod("create_variable_assignment_callback")
}

#' @export
create_variable_assignment_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_variable_assignment_create_from_r_function, object)
}

#' @export
create_variable_assignment_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_assignment_create_from_c_function, object)
}
