
#' @export
is_active <- function(object, ...) {
    UseMethod("is_active")
}

#' @export
is_active.instrumentr_call <- function(object, ...) {
    .Call(C_call_is_active, object)
}

#' @export
is_active.instrumentr_callback <- function(object, ...) {
    .Call(C_callback_is_active, object)
}
