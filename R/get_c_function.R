
#' @export
get_c_function <- function(object, ...) {
    UseMethod("get_c_function")
}

#' @export
get_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_c_function, object)
}
