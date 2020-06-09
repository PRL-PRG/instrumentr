
undefined_object <- structure(new.env(parent=emptyenv()), class = c("lightr_undefined"))

#' @export
is_undefined <- function(obj) {
    .Call(C_lightr_is_undefined_object, obj)
}

#' @export
is_defined <- function(obj) {
    .Call(C_lightr_is_defined_object, obj)
}
