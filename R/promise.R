#' API for promise objects
#'
#' @description
#' TODO
#'
#' @name promise
NULL


#' @export
#' @rdname promise
is_forced.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_is_forced, object)
}

#' @export
#' @rdname promise
get_expression.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_get_expression, object)
}

#' @export
#' @rdname promise
get_value.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_get_value, object)
}

#' @export
#' @rdname promise
get_type.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_get_type, object)
}

#' @export
#' @rdname promise
is_argument.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_is_argument, object)
}

#' @export
#' @rdname promise
is_delayed_assign.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_is_delayed_assign, object)
}

#' @export
#' @rdname promise
is_lazy_load.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_is_lazy_load, object)
}

#' @export
#' @rdname promise
is_unknown.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_is_unknown, object)
}

#' @export
#' @rdname promise
get_call.instrumentr_promise <- function(object) {
    .Call(r_instrumentr_promise_get_call, object)
}
