
#' @export
get_directory <- function(object, ...) {
    UseMethod("get_directory")
}

#' @export
get_directory.lightr_package <- function(object, ...) {
    .Call(C_package_get_directory, object)
}
