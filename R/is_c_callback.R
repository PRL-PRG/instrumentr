
#' @export
is_c_callback <- function(object, ...) {
    UseMethod("is_c_callback")
}

#' @export
is_c_callback.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_callback_is_c_callback, object)
}
