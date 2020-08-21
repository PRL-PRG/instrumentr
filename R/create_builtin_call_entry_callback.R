
#' @export
create_builtin_call_entry_callback <- function(object) { # nolint
    UseMethod("create_builtin_call_entry_callback")
}

#' @export
create_builtin_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_builtin_call_entry_callback_create_from_r_function, object)
}

#' @export
create_builtin_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_builtin_call_entry_callback_create_from_c_function, object)
}
