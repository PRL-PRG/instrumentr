
#' @export
get_name.instrumentr_argument <- function(object, ...) {
    .Call(C_instrumentr_argument_get_name, object)
}

#' @export
get_expression.instrumentr_argument <- function(object, ...) { # nolint
    .Call(C_instrumentr_argument_get_expression, object)
}

#' @export
print.instrumentr_argument <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
get_value.instrumentr_argument <- function(object, ...) { # nolint
    .Call(C_instrumentr_argument_get_value, object)
}

#' @export
is_evaluated.instrumentr_argument <- function(argument, ...) { # nolint
    .Call(C_instrumentr_argument_is_evaluated, argument)
}
