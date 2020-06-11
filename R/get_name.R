
#' @export
get_name <- function(object, ...) {
    UseMethod("get_name")
}

#' @export
get_name.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_application_get_name, object)
}

#' @export
get_name.instrumentr_package <- function(object, ...) {
    .Call(C_package_get_name, object)
}

#' @export
get_name.instrumentr_function <- function(object, ...) {
    .Call(C_function_get_name, object)
}

#' @export
get_name.instrumentr_parameter <- function(object, ...) {
    .Call(C_parameter_get_name, object)
}

#' @export
get_name.instrumentr_argument <- function(object, ...) {
    .Call(C_argument_get_name, object)
}
