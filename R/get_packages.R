
#' @export
get_packages <- function(application, ...) {
    UseMethod("get_packages")
}

#' @export
get_packages.lightr_application <- function(application, ...) {
    .Call(C_application_get_packages, application)
}
