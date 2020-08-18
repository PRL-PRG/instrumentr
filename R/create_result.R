
create_result <- function(object, ...) {
    UseMethod("create_result")
}

create_result.default <- function(object, ...) {
    structure(list(error = undefined_object, value = object), class = "instrumentr_result")
}

create_result.error <- function(object, source, ...) {
    error <- create_error(source, object$message, object$call)
    structure(list(error = error, value = undefined_object), class = "instrumentr_result")
}
