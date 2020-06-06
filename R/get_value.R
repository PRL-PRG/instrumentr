
#' @export
get_value <- function(object, ...) {
    UseMethod("get_value")
}

#' @export
get_value.lightr_result <- function(object, ...) {
    object$value
}
