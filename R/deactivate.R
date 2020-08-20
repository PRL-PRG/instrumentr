
#' @export
deactivate <- function(object, ...) {
    UseMethod("deactivate")
}

#' @export
deactivate.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_callback_deactivate, object)
}
