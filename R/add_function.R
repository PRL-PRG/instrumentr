
#' @export
add_function <- function(package, func) {
    stopifnot(is_lightr_package(package))
    stopifnot(is_lightr_function(func))
    .Call(C_package_add_function, package, func)
}
