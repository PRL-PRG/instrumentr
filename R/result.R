
create_result.default <- function(object, ...) {
    structure(list(error = NULL,
                   value = object),
              class = "instrumentr_result")
}

create_result.error <- function(object, source, ...) {
    error <- create_error(source, object$message, object$call)
    structure(list(error = error,
                   value = NULL),
              class = "instrumentr_result")
}

#' @export
get_error.instrumentr_result <- function(object) {
    object$error
}

#' @export
get_value.instrumentr_result <- function(object) {
    object$value
}

#' @export
is_error.instrumentr_result <- function(object) {
    is.null(get_error(object))
}

#' @export
is_value.instrumentr_result <- function(object) {
    !is_error(object)
}
