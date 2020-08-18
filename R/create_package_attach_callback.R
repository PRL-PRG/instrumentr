
#' @export
create_package_attach_callback <- function(object) {
    UseMethod("create_package_attach_callback")
}

#' @export
create_package_attach_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_package_attach_callback_create_from_r_function, object)
}

#' @export
create_package_attach_callback.externalptr <- function(object) {
    .Call(C_package_attach_callback_create_from_c_function, object)
}
