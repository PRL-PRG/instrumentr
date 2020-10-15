
#' @export
enable <- function(object, ...) {
    UseMethod("enable")
}

#' @export
enable.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_enable, object)
}
