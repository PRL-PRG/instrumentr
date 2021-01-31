#' API for callback objects
#'
#' @description
#' TODO
#'
#' @name callback
NULL

#' @export
#' @rdname callback
get_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_c_function, object)
}

#' @export
#' @rdname callback
get_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_get_r_function, object)
}

#' @export
#' @rdname callback
get_exec_stats.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_get_exec_stats, object)
}

#' @export
#' @rdname callback
has_c_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_c_function, object)
}

#' @export
#' @rdname callback
has_r_function.instrumentr_callback <- function(object, ...) { # nolint
    .Call(C_instrumentr_callback_has_r_function, object)
}

#' @export
#' @rdname callback
is_active.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_is_active, object)
}
