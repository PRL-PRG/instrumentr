
#' @export
add_package <- function(application, package) {
    stopifnot(is_lightr_application(application))
    stopifnot(is_lightr_package(package))
    .Call(C_application_add_package, application, package)
}
