
#' @export
get_call <- function(object, ...) {
    UseMethod("get_call")
}

#' @export
get_call.instrumentr_error <- function(object, ...) {
    object$call
}
