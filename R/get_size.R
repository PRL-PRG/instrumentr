#' @export
get_size <- function(call_stack, ...) {
    UseMethod("get_size")
}

#' @export
get_size.injectr_call_stack <- function(call_stack, ...) {
    .Call(C_call_stack_get_size, call_stack)
}
