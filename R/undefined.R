
undefined_object <- structure(new.env(parent=emptyenv()), class = c("instrumentr_undefined"))

#' @export
is_undefined <- function(obj) {
    .Call(C_instrumentr_is_undefined_object, obj)
}

#' @export
is_defined <- function(obj) {
    .Call(C_instrumentr_is_defined_object, obj)
}
