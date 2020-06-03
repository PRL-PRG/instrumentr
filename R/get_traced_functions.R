

#' @export
get_traced_functions <- function(object, package_name, ...) {
    UseMethod("get_traced_functions")
}

#' @export
get_traced_functions.lightr_context <- function(object, package_name, ...) {
    stopifnot(is_scalar_character(package_name))

    if(!is_package_traced(package_name)) {
        stop(sprintf("package %s is not traced.", package_name))
    }

    .Call(C_context_get_traced_functions, object, package_name)
}
