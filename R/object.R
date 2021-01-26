
#' @export
get_id.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_get_id, object)
}
