
#' @export
get_result <- function(object, ...) {
    UseMethod("get_result")
}

#' @export
get_result.instrumentr_call <- function(object, ...) {
    .Call(C_instrumentr_call_get_result, object)
}
