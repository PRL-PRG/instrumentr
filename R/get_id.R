
#' @export
get_id <- function(object, ...) {
    UseMethod("get_id")
}

#' @export
get_id.instrumentr_object <- function(object, ...) {
    .Call(C_object_get_id, object)
}
