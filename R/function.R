
#' @export
create_function <- function(function_name, parameter_count, function_object, public, s3_generic, s3_method) {
    stopifnot(is_scalar_character(function_name))
    stopifnot(is_scalar_integer(parameter_count))
    stopifnot(is_closure(function_object))
    stopifnot(is_scalar_logical(public))
    stopifnot(is_scalar_logical(s3_generic))
    stopifnot(is_scalar_logical(s3_method))

    .Call(C_instrumentr_function_create_closure,
          function_name,
          parameter_count,
          function_object,
          public,
          s3_generic,
          s3_method)
}


#' @export
get_name.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_get_name, object)
}

#' @export
get_definition.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_get_definition, object)
}

#' @export
print.instrumentr_function <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
is_public.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_is_public, object)
}

#' @export
is_s3_method.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_is_s3_method, object)
}

#' @export
is_s3_generic.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_is_s3_generic, object)
}

#' @export
to_string.instrumentr_function <- function(object, ...) {
    template <- "Function(name='%s', parameter_count=%d, definition=%s, public=%s, s3_generic=%s, s3_method=%s)"
    representation <- sprintf(template,
                              get_name(object),
                              get_parameter_count(object),
                              to_string(get_definition(object)),
                              logical_to_string(is_public(object)),
                              logical_to_string(is_s3_generic(object)),
                              logical_to_string(is_s3_method(object)))

    representation
}

