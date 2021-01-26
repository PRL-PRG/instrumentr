
create_result.default <- function(object, ...) {
    structure(list(error = undefined_object, value = object), class = "instrumentr_result")
}

create_result.error <- function(object, source, ...) {
    error <- create_error(source, object$message, object$call)
    structure(list(error = error, value = undefined_object), class = "instrumentr_result")
}

#' @export
print.instrumentr_result <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
get_error.instrumentr_result <- function(object, ...) {
    object$error
}

#' @export
get_value.instrumentr_result <- function(object, ...) {
    object$value
}

#' @export
is_error.instrumentr_result <- function(object, ...) {
    is_defined(get_error(object))
}

#' @export
is_value.instrumentr_result <- function(object, ...) {
    is_defined(get_value(object))
}

#' @export
to_string.instrumentr_result <- function(object, ...) {

    representation <-
        if (is_value(object)) {
            sprintf("Result(value=%s)", to_string(get_value(object)))
        }
        else {
            sprintf("Result(error=%s)", to_string(get_error(object)))
        }

    representation
}
