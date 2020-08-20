
#' @export
get_function <- function(object, ...) {
    UseMethod("get_function")
}

#' @export
get_function.instrumentr_call <- function(object, ...) {
    .Call(C_call_get_function, object)
}

#' @export
get_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_callback_get_function, object)
}
