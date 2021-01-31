
undefined_object <- structure(new.env(parent=emptyenv()), class = c("instrumentr_undefined"))

#' @export
is_undefined <- function(obj) {
    identical(obj, undefined_object)
}

#' @export
is_defined <- function(obj) {
    !is_undefined(obj)
}
