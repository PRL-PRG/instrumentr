
#' @export
is_missing <- function(object, ...) {
    UseMethod("is_missing")
}

#' @export
is_missing.lightr_parameter <- function(object, ...) {
    .Call(C_parameter_is_missing, object)
}
