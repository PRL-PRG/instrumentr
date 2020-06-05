
#' @export
get_definition <- function(object, ...) {
    UseMethod("get_definition")
}

#' @export
get_definition.lightr_function <- function(object, ...) {
    .Call(C_function_get_definition, object)
}
