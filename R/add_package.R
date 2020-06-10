
#' @export
add_package <- function(application, package) {
    stopifnot(is_instrumentr_application(application))
    stopifnot(is_instrumentr_package(package))
    .Call(C_application_add_package, application, package)
}
