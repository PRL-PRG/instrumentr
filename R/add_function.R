
#' @export
add_function <- function(package, func) {
    stopifnot(is_instrumentr_package(package))
    stopifnot(is_instrumentr_function(func))
    .Call(C_package_add_function, package, func)
}
