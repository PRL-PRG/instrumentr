
#' @export
get_function <- function(object, ...) {
    UseMethod("get_function")
}

#' @export
get_function.lightr_call <- function(object, ...) {
    .Call(C_call_get_function, object)
}
