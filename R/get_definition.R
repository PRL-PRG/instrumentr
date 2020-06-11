
#' @export
get_definition <- function(object, ...) {
    UseMethod("get_definition")
}

#' @export
get_definition.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_function_get_definition, object)
}
