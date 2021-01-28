
create_error <- function(source, message, call) {
    structure(list(source = source, message = message, call = call),
              class = c("instrumentr_error", "simpleError", "error", "condition"))
}

#' @export
get_call.instrumentr_error <- function(object, ...) {
    object$call
}

#' @export
get_message.instrumentr_error <- function(object, ...) {
    object$message
}

#' @export
get_source.instrumentr_error <- function(object, ...) {
    object$source
}
