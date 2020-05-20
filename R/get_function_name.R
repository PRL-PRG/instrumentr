
#' @export
get_function_name <- function(call, ...) {
    UseMethod("get_function_name")
}

#' @export
get_function_name.lightr_call <- function(call, ...) {
    .Call(C_call_get_function_name, call)
}
