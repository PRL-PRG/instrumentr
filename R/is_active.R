
#' @export
is_active <- function(object, ...) {
    UseMethod("is_active")
}

#' @export
is_active.lightr_call <- function(object, ...) {
    .Call(C_call_is_active, object)
}
