#' API for function objects
#'
#' @description
#' TODO
#'
#' @name function
NULL

#' @export
#' @rdname function
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
#' @rdname function
get_name.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_get_name, object)
}

#' @export
#' @rdname function
get_definition.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_get_definition, object)
}

#' @export
#' @rdname function
is_public.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_is_public, object)
}

#' @export
#' @rdname function
is_s3_method.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_is_s3_method, object)
}

#' @export
#' @rdname function
is_s3_generic.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_is_s3_generic, object)
}

#' @export
#' @rdname function
get_parameter_count.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_get_parameter_count, object)
}
