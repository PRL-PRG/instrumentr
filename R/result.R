
create_result.default <- function(object, ...) {
    structure(list(error = undefined_object,
                   value = object,
                   output = NULL,
                   statistics = NULL),
              class = "instrumentr_result")
}

create_result.error <- function(object, source, ...) {
    error <- create_error(source, object$message, object$call)
    structure(list(error = error,
                   value = undefined_object,
                   output = NULL,
                   statistics = NULL),
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
    is_defined(get_error(object))
}

#' @export
is_value.instrumentr_result <- function(object) {
    is_defined(get_value(object))
}

#' @export
set_state.instrumentr_result <- function(object, state) {
    object$output <- state$output
    object$statistics <- state$statistics
    object
}

#' @export
get_state.instrumentr_result <- function(object) {
    object$state
}

