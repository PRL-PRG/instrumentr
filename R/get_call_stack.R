
#' @export
get_call_stack <- function(object, ...) {
    UseMethod("get_call_stack")
}

#' @export
get_call_stack.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_call_stack, object)
}
