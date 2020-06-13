
#' @export
is_s3_generic <- function(object, ...) {
    UseMethod("is_s3_generic")
}

#' @export
is_s3_generic.instrumentr_function <- function(object, ...) {
    .Call(C_function_is_s3_generic, object)
}
