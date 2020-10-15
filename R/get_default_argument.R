
#' @export
get_default_argument <- function(parameter, ...) {
    UseMethod("get_default_argument")
}

#' @export
get_default_argument.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_instrumentr_parameter_get_default_argument, parameter)
}
