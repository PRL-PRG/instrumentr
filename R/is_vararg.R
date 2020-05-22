
#' @export
is_vararg <- function(object, ...) {
    UseMethod("is_vararg")
}

#' @export
is_vararg.lightr_parameter <- function(object, ...) {
    .Call(C_parameter_is_vararg, object)
}
