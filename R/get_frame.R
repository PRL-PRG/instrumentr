#' @export
get_frame <- function(call_stack, position, ...) {
    UseMethod("get_frame")
}

#' @export
get_frame.lightr_call_stack <- function(call_stack, position, ...) {
    .Call(C_call_stack_get_frame, call_stack, position)
}
