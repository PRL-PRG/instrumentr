
#' @export
get_value <- function(object, ...) {
    UseMethod("get_value")
}

#' @export
get_value.instrumentr_result <- function(object, ...) {
    object$value
}

#' @export
get_value.instrumentr_argument <- function(object, ...) { # nolint
    .Call(C_instrumentr_argument_get_value, object)
}
