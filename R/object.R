#' API for all objects
#'
#' @description
#' instrumentr creates model objects for tracing. These model objects reflect
#' the state of the application as it is being traced and provide convenient
#' handles for accessing information.
#' Model objects created by instrumentr share some common features.
#'
#' @details
#'
#' ## Id
#' All instrumentr model objects have a unique immutable integer id assigned by
#' instrumentr. This can be accessed using the `get_id` function.
#'
#' @family object api
#'
#' @examples
#'
#' @name object
NULL

#' @export
#' @rdname object
get_id <- function(object) {
    UseMethod("get_id")
}

#' @export
get_id.instrumentr_object <- function(object) {
    .Call(C_instrumentr_object_get_id, object)
}
