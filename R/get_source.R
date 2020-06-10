
#' @export
get_source <- function(object, ...) {
    UseMethod("get_source")
}

#' @export
get_source.instrumentr_error <- function(object, ...) {
    object$source
}
