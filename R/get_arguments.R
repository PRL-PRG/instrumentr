
#' @export
get_arguments <- function(parameter, ...) {
    UseMethod("get_arguments")
}

#' @export
get_arguments.lightr_parameter <- function(parameter, ...) {
    .Call(C_parameter_get_arguments, parameter)
}
