
#' @export
get_packages <- function(application, ...) {
    UseMethod("get_packages")
}

#' @export
get_packages.instrumentr_application <- function(application, ...) { # nolint
    .Call(C_application_get_packages, application)
}
