#' @export
get_size <- function(call_stack, ...) {
    UseMethod("get_size")
}

#' @export
get_size.instrumentr_call_stack <- function(call_stack, ...) { # nolint
    .Call(C_instrumentr_call_stack_get_size, call_stack)
}
