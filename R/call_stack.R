#' API for Call Stack objects
#'
#' @description
#' TODO
#'
#' @name call-stack
NULL

#' @export
#' @rdname call-stack
get_size.instrumentr_call_stack <- function(object) { # nolint
    .Call(r_instrumentr_call_stack_get_size, object)
}

#' @export
#' @rdname call-stack
peek_frame.instrumentr_call_stack <- function(call_stack, position, ...) { # nolint
    .Call(r_instrumentr_call_stack_peek_frame, call_stack, position)
}
