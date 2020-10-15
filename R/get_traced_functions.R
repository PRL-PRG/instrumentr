

#' @export
get_traced_functions <- function(object, package_name, ...) {
    UseMethod("get_traced_functions")
}

#' @export
get_traced_functions.instrumentr_tracer <- function(object, package_name, ...) { # nolint
    stopifnot(is_scalar_character(package_name))

    if (!is_package_traced(object, package_name)) {
        stop(sprintf("package %s is not traced.", package_name))
    }

    .Call(C_instrumentr_tracer_get_traced_functions, object, package_name)
}
