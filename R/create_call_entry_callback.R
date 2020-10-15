
#' @export
create_call_entry_callback <- function(object) {
    UseMethod("create_call_entry_callback")
}

#' @export
create_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_instrumentr_callback_call_entry_create_from_r_function, object)
}

#' @export
create_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_call_entry_create_from_c_function, object)
}
