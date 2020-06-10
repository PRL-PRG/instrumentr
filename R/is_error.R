
#' @export
is_error <- function(object, ...) {
    UseMethod("is_error")
}

#' @export
is_error.instrumentr_result <- function(object, ...) {
    is_defined(get_error(object))
}
