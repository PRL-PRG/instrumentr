
#' @export
get_name <- function(object, ...) {
    UseMethod("get_name")
}

#' @export
get_name.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_name, object)
}

#' @export
get_name.instrumentr_package <- function(object, ...) {
    .Call(C_instrumentr_package_get_name, object)
}

#' @export
get_name.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_get_name, object)
}

#' @export
get_name.instrumentr_parameter <- function(object, ...) {
    .Call(C_instrumentr_parameter_get_name, object)
}

#' @export
get_name.instrumentr_argument <- function(object, ...) {
    .Call(C_instrumentr_argument_get_name, object)
}
