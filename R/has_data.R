
#' @export
has_data <- function(object, ...) {
    UseMethod("has_data")
}

#' @export
has_data.instrumentr_object <- function(object, ...) {
    .Call(C_object_has_data, object)
}
