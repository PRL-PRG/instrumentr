
#' @export
create_application_load_callback <- function(object) {
    UseMethod("create_application_load_callback")
}

#' @export
create_application_load_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_load_callback_create_from_r_function, object)
}

#' @export
create_application_load_callback.externalptr <- function(object) {
    .Call(C_application_load_callback_create_from_c_function, object)
}
