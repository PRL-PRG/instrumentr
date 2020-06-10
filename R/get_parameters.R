
#' @export
get_parameters <- function(call, ...) {
    UseMethod("get_parameters")
}

#' @export
get_parameters.instrumentr_call <- function(call, ...) {
    .Call(C_call_get_parameters, call)
}
