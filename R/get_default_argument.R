
#' @export
get_default_argument <- function(func, position_or_name, ...) {
    UseMethod("get_default_argument")
}

#' @export
get_default_argument.instrumentr_function <- function(func, position_or_name, ...) { # nolint

    if (is_scalar_character(position_or_name)) {
        .Call(C_function_get_default_argument_by_name, func, position_or_name)
    }

    else if (is_scalar_integer(position_or_name)) {
        .Call(C_function_get_default_argument_by_position, func, position_or_name)
    }

    else if (is_scalar_real(position_or_name)) {
        position_or_name <- as.integer(position_or_name)
        .Call(C_function_get_default_argument_by_position, func, position_or_name)
    }

    else {
        message <- sprintf("unexpected 'position_or_name' value of type '%s'", typeof(position_or_name))
        stop(message)
    }
}
