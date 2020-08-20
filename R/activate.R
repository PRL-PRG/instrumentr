
#' @export
activate <- function(object, ...) {
    UseMethod("activate")
}

#' @export
activate.instrumentr_callback <- function(object, ...) {
    .Call(C_callback_activate, object)
}
