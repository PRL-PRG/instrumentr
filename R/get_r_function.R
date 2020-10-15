
#' @export
get_r_function <- function(object, ...) {
    UseMethod("get_r_function")
}

#' @export
get_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_r_function, object)
}
