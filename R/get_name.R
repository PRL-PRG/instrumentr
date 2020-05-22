
#' @export
get_name <- function(object, ...) {
    UseMethod("get_name")
}

#' @export
get_name.lightr_package <- function(object, ...) {
    .Call(C_package_get_name, object)
}

#' @export
get_name.lightr_function <- function(object, ...) {
    .Call(C_function_get_name, object)
}

#' @export
get_name.lightr_parameter <- function(object, ...) {
    .Call(C_parameter_get_name, object)
}

#' @export
get_name.lightr_argument <- function(object, ...) {
    .Call(C_argument_get_name, object)
}
