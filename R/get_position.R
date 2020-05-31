
#' @export
get_position <- function(object, ...) {
    UseMethod("get_position")
}

#' @export
get_position.lightr_parameter <- function(object, ...) {
    .Call(C_parameter_get_position, object)
}
