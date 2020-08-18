
#' @export
create_application_unload_callback <- function(object) {
    UseMethod("create_application_unload_callback")
}

#' @export
create_application_unload_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_unload_callback_create_from_r_function, object)
}

#' @export
create_application_unload_callback.externalptr <- function(object) {
    .Call(C_application_unload_callback_create_from_c_function, object)
}
