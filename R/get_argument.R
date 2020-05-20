
#' @export
get_argument <- function(call, position, ...) {
    UseMethod("get_argument")
}

#' @export
get_argument.lightr_call <- function(call, position, ...) {
    .Call(C_call_get_argument, position)
}
