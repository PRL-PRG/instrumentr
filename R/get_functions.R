
#' @export
get_functions <- function(object, ...) {
    UseMethod("get_functions")
}

#' @export
get_functions.lightr_package <- function(object, ...) {
    .Call(C_package_get_functions, object)
}
