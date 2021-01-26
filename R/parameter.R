
#' @export
get_name.instrumentr_parameter <- function(object, ...) {
    .Call(C_instrumentr_parameter_get_name, object)
}

#' @export
get_argument.instrumentr_parameter <- function(parameter, position_or_name, ...) { # nolint
    if (is_scalar_character(position_or_name)) {
        .Call(C_instrumentr_parameter_get_argument_by_name, parameter, position_or_name)
    }

    else if (is_scalar_integer(position_or_name)) {
        .Call(C_instrumentr_parameter_get_argument_by_position, parameter, position_or_name)
    }

    else if (is_scalar_real(position_or_name)) {
        position_or_name <- as.integer(position_or_name)
        .Call(C_instrumentr_parameter_get_argument_by_position, parameter, position_or_name)
    }

    else {
        message <- sprintf("unexpected 'position_or_name' value of type '%s'", typeof(position_or_name))
        stop(message)
    }
}

#' @export
get_arguments.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_instrumentr_parameter_get_arguments, parameter)
}

#' @export
print.instrumentr_parameter <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
get_default_argument.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_instrumentr_parameter_get_default_argument, parameter)
}

#' @export
get_position.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_instrumentr_parameter_get_position, object)
}

#' @export
is_missing.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_instrumentr_parameter_is_missing, object)
}

#' @export
is_vararg.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_instrumentr_parameter_is_vararg, object)
}

#' @export
has_default_argument.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_instrumentr_parameter_has_default_argument, parameter)
}

#' @export
to_string.instrumentr_parameter <- function(object, ...) { # nolint
    representation <- sprintf("Parameter(name='%s', position=%d, missing=%s, vararg=%s)",
                              get_name(object),
                              get_position(object),
                              logical_to_string(is_missing(object)),
                              logical_to_string(is_vararg(object)))

    representation
}
