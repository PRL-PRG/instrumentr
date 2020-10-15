
#' @export
reinstate <- function(object, ...) {
    UseMethod("reinstate")
}

#' @export
reinstate.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_reinstate, object)
}
