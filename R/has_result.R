
#' @export
has_result <- function(object, ...) {
    UseMethod("has_result")
}

#' @export
has_result.instrumentr_call <- function(object, ...) {
    .Call(C_instrumentr_call_has_result, object)
}
