
#' @export
remove_data <- function(object, ...) {
    UseMethod("remove_data")
}

#' @export
remove_data.lightr_object <- function(object, ...) {
    .Call(C_object_remove_data, object)
    invisible(NULL)
}
