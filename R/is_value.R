
#' @export
is_value <- function(object, ...) {
    UseMethod("is_value")
}

#' @export
is_value.instrumentr_result <- function(object, ...) {
    is_defined(get_value(object))
}
