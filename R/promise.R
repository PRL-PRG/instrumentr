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
    .Call(C_instrumentr_promise_is_forced, object)
}

#' @export
#' @rdname promise
get_expression.instrumentr_promise <- function(object) {
    .Call(C_instrumentr_promise_get_expression, object)
}

#' @export
#' @rdname promise
get_value.instrumentr_promise <- function(object) {
    .Call(C_instrumentr_promise_get_value, object)
}
