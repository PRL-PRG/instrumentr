
#' @export
get_error <- function(object, ...) {
    UseMethod("get_error")
}

#' @export
get_error.lightr_result <- function(object, ...) {
    object$error
}
