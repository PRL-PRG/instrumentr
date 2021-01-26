
#' @export
get_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_c_function, object)
}

#' @export
get_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_r_function, object)
}

#' @export
get_exec_stats.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_get_exec_stats, object)
}

#' @export
has_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_c_function, object)
}

#' @export
has_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_r_function, object)
}

#' @export
is_active.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_is_active, object)
}
