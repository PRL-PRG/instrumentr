
#' @export
get_message <- function(object, ...) {
    UseMethod("get_message")
}

#' @export
get_message.instrumentr_error <- function(object, ...) {
    object$message
}
