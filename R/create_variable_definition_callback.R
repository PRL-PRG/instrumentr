
#' @export
create_variable_definition_callback <- function(object) {
    UseMethod("create_variable_definition_callback")
}

#' @export
create_variable_definition_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_variable_definition_callback_create_from_r_function, object)
}

#' @export
create_variable_definition_callback.externalptr <- function(object) {
    .Call(C_variable_definition_callback_create_from_c_function, object)
}
