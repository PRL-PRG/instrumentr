
#' @export
get_call <- function(argument, ...) {
    UseMethod("get_call")
}

#' @export
get_call.lightr_argument <- function(argument, ...) {
    .Call(C_argument_get_call, argument)
}
