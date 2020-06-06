
create_result <- function(object, ...) {
    UseMethod("create_result")
}

create_result.default <- function(object, ...) {
    structure(list(error = invalid_value, value = object), class = "lightr_result")
}

create_result.error <- function(object, execution_context, ...) {
    error <- create_error(execution_context, object$message, object$call)
    structure(list(error = error, value = invalid_value), class = "lightr_result")
}
