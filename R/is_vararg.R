
#' @export
is_vararg <- function(object, ...) {
    UseMethod("is_vararg")
}

#' @export
is_vararg.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_parameter_is_vararg, object)
}
