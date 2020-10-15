
#' @export
create_eval_entry_callback <- function(object) {
    UseMethod("create_eval_entry_callback")
}

#' @export
create_eval_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_eval_entry_create_from_r_function, object)
}

#' @export
create_eval_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_eval_entry_create_from_c_function, object)
}
