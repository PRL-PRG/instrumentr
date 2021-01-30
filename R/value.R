#' API for value objects
#'
#' @description
#' TODO
#'
#' @name value
NULL

#' @export
#' @rdname value
get_sexp.instrumentr_value <- function(object) {
    .Call(C_instrumentr_value_get_sexp, object)
}

#' @export
#' @rdname value
get_sexp_address.instrumentr_value <- function(object) {
    .Call(C_instrumentr_value_get_sexp_address, object)
}

#' @export
#' @rdname value
get_sexp_type.instrumentr_value <- function(object) {
    .Call(C_instrumentr_value_get_sexp_type, object)
}
