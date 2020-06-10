
#' @export
get_data <- function(object, ...) {
    UseMethod("get_data")
}

#' @export
get_data.instrumentr_object <- function(object, ...) {
    .Call(C_object_get_data, object)
}
