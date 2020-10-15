
#' @export
has_c_function <- function(object, ...) {
    UseMethod("has_c_function")
}

#' @export
has_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_c_function, object)
}
