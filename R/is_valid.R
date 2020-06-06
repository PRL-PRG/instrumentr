
#' @export
is_valid <- function(object, ...) {
    UseMethod("is_valid")
}

#' @export
is_valid.lightr_result <- function(object, ...) {
    is_invalid_value(get_error(object))
}
