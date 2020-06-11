
#' @export
get_position <- function(object, ...) {
    UseMethod("get_position")
}

#' @export
get_position.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_parameter_get_position, object)
}
