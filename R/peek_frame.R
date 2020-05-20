#' @export
peek_frame <- function(call_stack, position, ...) {
    UseMethod("peek_frame")
}

#' @export
peek_frame.injectr_call_stack <- function(call_stack, position, ...) {
    .Call(C_call_stack_peek_frame, call_stack, position)
}
