#' API for callback objects
#'
#' @description
#' TODO
#'
#' @name callback
NULL

#' @export
#' @rdname callback
is_callback <- function(object) {
    inherits(object, "instrumentr_callback")
}

#' @export
create_callback.function <- function(object, state, event) { # nolint
    .Call(C_instrumentr_callback_create_from_r_function, state, object, event)
}

#' @export
create_callback.externalptr <- function(object, state, event) { # nolint
    .Call(C_instrumentr_callback_create_from_c_function, state, object, event)
}

#' @export
#' @rdname callback
get_event.instrumentr_callback <- function(object) { # nolint
    .Call(C_instrumentr_callback_get_event, object)
}

#' @export
#' @rdname callback
handles_event.instrumentr_callback <- function(object, event) { # nolint
    .Call(C_instrumentr_callback_handles_event, object, event)
}

#' @export
#' @rdname callback
get_c_function.instrumentr_callback <- function(object) { # nolint
    .Call(C_instrumentr_callback_get_c_function, object)
}

#' @export
#' @rdname callback
get_r_function.instrumentr_callback <- function(object) { # nolint
    .Call(C_instrumentr_callback_get_r_function, object)
}

#' @export
#' @rdname callback
has_c_function.instrumentr_callback <- function(object) { # nolint
    .Call(C_instrumentr_callback_has_c_function, object)
}

#' @export
#' @rdname callback
has_r_function.instrumentr_callback <- function(object) { # nolint
    .Call(C_instrumentr_callback_has_r_function, object)
}

#' @export
#' @rdname callback
is_active.instrumentr_callback <- function(object) {
    .Call(C_instrumentr_callback_is_active, object)
}
