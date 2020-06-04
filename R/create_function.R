
#' @export
create_function <- function(function_name, parameter_count, function_object) {
    stopifnot(is_scalar_character(function_name))
    stopifnot(is_scalar_integer(parameter_count))
    stopifnot(is_closure(function_object))

    .Call(C_function_create_function, function_name, parameter_count, function_object)
}
