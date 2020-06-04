
#' @export
get_object <- function(object, ...) {
    UseMethod("get_object")
}

#' @export
get_object.lightr_function <- function(object, ...) {
    .Call(C_function_get_object, object)
}

#' @export
get_object.lightr_call <- function(object, ...) {
    .Call(C_call_get_object, object)
}
