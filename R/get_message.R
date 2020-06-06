
#' @export
get_message <- function(object, ...) {
    UseMethod("get_message")
}

#' @export
get_message.lightr_error <- function(object, ...) {
    object$message
}
