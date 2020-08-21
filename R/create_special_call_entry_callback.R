
#' @export
create_special_call_entry_callback <- function(object) { # nolint
    UseMethod("create_special_call_entry_callback")
}

#' @export
create_special_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_special_call_entry_callback_create_from_r_function, object)
}

#' @export
create_special_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_special_call_entry_callback_create_from_c_function, object)
}
