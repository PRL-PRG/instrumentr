
#' @export
get_arguments <- function(parameter, ...) {
    UseMethod("get_arguments")
}

#' @export
get_arguments.instrumentr_parameter <- function(parameter, ...) { # nolint
    .Call(C_parameter_get_arguments, parameter)
}
