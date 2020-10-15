
#' @export
disable <- function(object, ...) {
    UseMethod("disable")
}

#' @export
disable.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_disable, object)
}
