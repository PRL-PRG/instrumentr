
#' @export
is_function_traced <- function(object, package_name, function_name, ...) {
    UseMethod("is_function_traced")
}

#' @export
is_function_traced.lightr_context <- function(object, package_name, function_name, ...) {

    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(function_name))

    .Call(C_context_is_function_traced, object, package_name, function_name)
}