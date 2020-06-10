
#' @export
get_error <- function(object, ...) {
    UseMethod("get_error")
}

#' @export
get_error.instrumentr_result <- function(object, ...) {
    object$error
}
