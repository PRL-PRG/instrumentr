
#' @export
create_eval_exit_callback <- function(object) {
    UseMethod("create_eval_exit_callback")
}

#' @export
create_eval_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_eval_exit_callback_create_from_r_function, object)
}

#' @export
create_eval_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_eval_exit_callback_create_from_c_function, object)
}
