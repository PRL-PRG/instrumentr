
#' @export
get_value <- function(object, ...) {
    UseMethod("get_value")
}

#' @export
get_value.instrumentr_result <- function(object, ...) {
    object$value
}
