#' API for argument objects
#'
#' @description
#' TODO
#'
#' @name argument
NULL

#' @export
#' @rdname argument
has_name.instrumentr_argument <- function(object) {
    .Call(r_instrumentr_argument_has_name, object)
}

#' @export
#' @rdname argument
get_name.instrumentr_argument <- function(object) {
    .Call(r_instrumentr_argument_get_name, object)
}

#' @export
#' @rdname argument
is_value.instrumentr_argument <- function(object) { # nolint
    .Call(r_instrumentr_argument_is_value, object)
}

#' @export
#' @rdname argument
as_value.instrumentr_argument <- function(object) { # nolint
    .Call(r_instrumentr_argument_as_value, object)
}

#' @export
#' @rdname argument
is_promise.instrumentr_argument <- function(object) { # nolint
    .Call(r_instrumentr_argument_is_promise, object)
}

#' @export
#' @rdname argument
as_promise.instrumentr_argument <- function(object) { # nolint
    .Call(r_instrumentr_argument_as_promise, object)
}

