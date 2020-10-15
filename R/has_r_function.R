
#' @export
has_r_function <- function(object, ...) {
    UseMethod("has_r_function")
}

#' @export
has_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_r_function, object)
}
