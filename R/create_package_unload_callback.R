
#' @export
create_package_unload_callback <- function(object) { # nolint
    UseMethod("create_package_unload_callback")
}

#' @export
create_package_unload_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_package_unload_create_from_r_function, object)
}

#' @export
create_package_unload_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_package_unload_create_from_c_function, object)
}
