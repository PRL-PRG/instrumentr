
#' @export
get_parameter <- function(call, ...) {
    UseMethod("get_parameter")
}

#' @export
get_parameter.instrumentr_call <- function(call, position_or_name, ...) { # nolint
    if (is_scalar_character(position_or_name)) {
        .Call(C_instrumentr_call_get_parameter_by_name, call, position_or_name)
    }

    else if (is_scalar_integer(position_or_name)) {
        .Call(C_instrumentr_call_get_parameter_by_position, call, position_or_name)
    }

    else if (is_scalar_real(position_or_name)) {
        position_or_name <- as.integer(position_or_name)
        .Call(C_instrumentr_call_get_parameter_by_position, call, position_or_name)
    }

    else {
        message <- sprintf("unexpected 'position_or_name' value of type '%s'", typeof(position_or_name))
        stop(message)
    }
}
