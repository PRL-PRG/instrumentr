
#' @export
is_r_callback <- function(object, ...) {
    UseMethod("is_r_callback")
}

#' @export
is_r_callback.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_callback_is_r_callback, object)
}
