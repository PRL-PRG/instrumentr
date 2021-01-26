
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

#' @export
to_string.instrumentr_argument <- function(object, ...) {
    name <- get_name(object)

    name_str <- ""
    if (name != "") {
        name_str <- sprintf("name='%s', ", name)
    }

    result <- get_result(object)
    result_str <- ""
    if (is_defined(result)) {
        result_str <- sprintf("result=%s, ", to_string(result))
    }

    representation <- sprintf("Argument(%sexpression=%s, %sevaluated=%s)",
                              name_str,
                              to_string(get_expression(object)),
                              result_str,
                              c("FALSE", "TRUE")[is_evaluated(object) + 1])

    representation
}
