
#' @export
has_default_argument <- function(parameter, ...) {
    UseMethod("has_default_argument")
}

#' @export
has_default_argument.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_instrumentr_parameter_has_default_argument, parameter)
}
