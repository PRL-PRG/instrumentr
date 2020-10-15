
#' @export
create_function <- function(function_name, parameter_count, function_object, public, s3_generic, s3_method) {
    stopifnot(is_scalar_character(function_name))
    stopifnot(is_scalar_integer(parameter_count))
    stopifnot(is_closure(function_object))
    stopifnot(is_scalar_logical(public))
    stopifnot(is_scalar_logical(s3_generic))
    stopifnot(is_scalar_logical(s3_method))

    .Call(C_instrumentr_function_create,
          function_name,
          parameter_count,
          function_object,
          public,
          s3_generic,
          s3_method)
}
