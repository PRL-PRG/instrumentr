
#' @export
is_successful <- function(object, ...) {
    UseMethod("is_successful")
}

#' @export
is_successful.lightr_call <- function(object, ...) {
    .Call(C_call_is_successful, object)
}
