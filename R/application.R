
#' @export
add_package <- function(object, package, ...) {
    UseMethod("add_package")
}

#' @export
add_package.instrumentr_application <- function(object, package, ...) {
    .Call(C_instrumentr_application_add_package, object, package)
}
