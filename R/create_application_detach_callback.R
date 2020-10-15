
#' @export
create_application_detach_callback <- function(object) { # nolint
    UseMethod("create_application_detach_callback")
}

#' @export
create_application_detach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_application_detach_create_from_r_function, object)
}

#' @export
create_application_detach_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_application_detach_create_from_c_function, object)
}
