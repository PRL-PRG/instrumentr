
#' @export
get_data <- function(object, ...) {
    UseMethod("get_data")
}

#' @export
get_data.lightr_object <- function(object, ...) {
    if(!has_data(object)) {
        stop(sprintf("%s does not have attached data", to_string(object)))
    }
    .Call(C_object_get_data, object)
}
