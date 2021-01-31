
#' @export
get_size.instrumentr_call_stack <- function(call_stack, ...) { # nolint
    .Call(C_instrumentr_call_stack_get_size, call_stack)
}

#' @export
peek.instrumentr_call_stack <- function(call_stack, position, ...) { # nolint
    .Call(C_instrumentr_call_stack_peek, call_stack, position)
}
