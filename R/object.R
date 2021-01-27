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
#' ## Data
#' instrumentr allows arbitrary R values to be attached to model objects.
#' This feature facilitates easy bookkeeping of tracing state. The attached data
#' is completely transparent to the tracing mechanism and is managed by
#' the user. By default, the model objects don't have any attached data.
#' `set_data` attaches data to a model object. This attached
#' data can then be retrieved using `get_data`. Attempting to access data from
#' an object without any attached data results in an error. Presence of attached
#' data can be queried using `has_data`.`remove_data` removes the attached data.
#'
#' It is advisable to attach environments as data to model objects. This is
#' because environments have reference semantics; updating them does not
#' create a new environment requiring reattachment using `set_data`.
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

#' @export
#' @rdname object
set_data <- function(object, data) {
    UseMethod("set_data")
}

#' @export
set_data.instrumentr_object <- function(object, data) {
    .Call(C_instrumentr_object_set_data, object, data)
    invisible(NULL)
}

#' @export
#' @rdname object
get_data <- function(object) {
    UseMethod("get_data")
}

#' @export
get_data.instrumentr_object <- function(object) {
    .Call(C_instrumentr_object_get_data, object)
}

#' @export
#' @rdname object
has_data <- function(object) {
    UseMethod("has_data")
}

#' @export
has_data.instrumentr_object <- function(object) {
    .Call(C_instrumentr_object_has_data, object)
}

#' @export
#' @rdname object
remove_data <- function(object) {
    UseMethod("remove_data")
}

#' @export
remove_data.instrumentr_object <- function(object) {
    .Call(C_instrumentr_object_remove_data, object)
    invisible(NULL)
}
