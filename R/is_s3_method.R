
#' @export
is_s3_method <- function(object, ...) {
    UseMethod("is_s3_method")
}

#' @export
is_s3_method.instrumentr_function <- function(object, ...) { # nolint
    .Call(C_function_is_s3_method, object)
}
