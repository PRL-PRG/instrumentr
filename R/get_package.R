
#' @export
get_package <- function(object, ...) {
    UseMethod("get_package")
}

#' @export
get_package.lightr_function <- function(object, ...) {
    .Call(C_function_get_package, object)
}
