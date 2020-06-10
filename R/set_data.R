
#' @export
set_data <- function(object, data, ...) {
    UseMethod("set_data")
}

#' @export
set_data.instrumentr_object <- function(object, data, ...) {
    .Call(C_object_set_data, object, data)
    invisible(NULL)
}
