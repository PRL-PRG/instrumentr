
#' @export
create_variable_removal_callback <- function(object) {
    UseMethod("create_variable_removal_callback")
}

#' @export
create_variable_removal_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_variable_removal_callback_create_from_r_function, object)
}

#' @export
create_variable_removal_callback.externalptr <- function(object) {
    .Call(C_variable_removal_callback_create_from_c_function, object)
}
