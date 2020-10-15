
#' @export
is_function_traced <- function(object, package_name, function_name, ...) {
    UseMethod("is_function_traced")
}

#' @export
is_function_traced.instrumentr_tracer <- function(object, package_name, function_name, ...) { # nolint

    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(function_name))

    .Call(C_instrumentr_tracer_is_function_traced, object, package_name, function_name)
}
