
#' @export
get_expression <- function(object, ...) {
    UseMethod("get_expression")
}

#' @export
get_expression.lightr_call <- function(object, ...) {
    .Call(C_call_get_expression, object)
}
