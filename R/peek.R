#' @export
peek <- function(call_stack, position, ...) {
    UseMethod("peek")
}

#' @export
peek.instrumentr_call_stack <- function(call_stack, position, ...) { # nolint
    .Call(C_instrumentr_call_stack_peek, call_stack, position)
}
