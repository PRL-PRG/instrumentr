
#' @export
get_argument <- function(parameter, ...) {
    UseMethod("get_argument")
}

#' @export
get_argument.instrumentr_parameter <- function(parameter, position_or_name, ...) { # nolint
    if (is_scalar_character(position_or_name)) {
        .Call(C_parameter_get_argument_by_name, parameter, position_or_name)
    }

    else if (is_scalar_integer(position_or_name)) {
        .Call(C_parameter_get_argument_by_position, parameter, position_or_name)
    }

    else if (is_scalar_real(position_or_name)) {
        position_or_name <- as.integer(position_or_name)
        .Call(C_parameter_get_argument_by_position, parameter, position_or_name)
    }

    else {
        message <- sprintf("unexpected 'position_or_name' value of type '%s'", typeof(position_or_name))
        stop(message)
    }
}
