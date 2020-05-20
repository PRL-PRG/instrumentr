
#' @export
get_id <- function(call, ...) {
    UseMethod("get_id")
}

#' @export
get_id.lightr_call <- function(call, ...) {
    .Call(C_call_get_id, call)
}
