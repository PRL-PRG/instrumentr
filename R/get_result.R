
#' @export
get_result <- function(object, ...) {
    UseMethod("get_result")
}

#' @export
get_result.instrumentr_call <- function(object, ...) {
    .Call(C_call_get_result, object)
}

#' @export
get_result.instrumentr_argument <- function(object, ...) { # nolint
    .Call(C_argument_get_result, object)
}
