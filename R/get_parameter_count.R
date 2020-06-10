
#' @export
get_parameter_count <- function(object, ...) {
    UseMethod("get_parameter_count")
}

#' @export
get_parameter_count.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_function_get_parameter_count, object)
}
