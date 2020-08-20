
#' @export
create_application_attach_callback <- function(object) { # nolint
    UseMethod("create_application_attach_callback")
}

#' @export
create_application_attach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_attach_callback_create_from_r_function, object)
}

#' @export
create_application_attach_callback.externalptr <- function(object) { # nolint
    .Call(C_application_attach_callback_create_from_c_function, object)
}
