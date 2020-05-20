
#' @export
get_state <- function(call, ...) {
    UseMethod("get_state")
}

#' @export
get_state.lightr_call <- function(call, ...) {
    .Call(C_call_get_state, call)
}
