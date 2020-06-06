
#' @export
get_call_stack <- function(object, ...) {
    UseMethod("get_call_stack")
}

#' @export
get_call_stack.lightr_application <- function(object, ...) { # nolint
    .Call(C_application_get_call_stack, object)
}
