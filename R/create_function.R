
#' @export
create_function <- function(function_name, parameter_count) {
    stopifnot(is_scalar_character(function_name))
    stopifnot(is_scalar_integer(parameter_count))
    .Call(C_function_create, function_name, parameter_count)
}
