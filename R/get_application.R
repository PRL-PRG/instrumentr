
#' @export
get_application <- function(package, ...) {
    UseMethod("get_application")
}

#' @export
get_application.lightr_package <- function(package, ...) {
    .Call(C_package_get_application, package)
}
