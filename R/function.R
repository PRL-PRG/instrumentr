#' API for function objects
#'
#' @description
#' TODO
#'
#' @name function
NULL

#' @export
#' @rdname function
has_name.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_has_name, object)
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
is_inner.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_is_inner, object)
}

#' @export
#' @rdname function
get_parent.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_instrumentr_function_get_parent, object)
}

#' @export
#' @rdname function
is_exported.instrumentr_function <- function(object, ...) {
    .Call(C_instrumentr_function_is_exported, object)
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
