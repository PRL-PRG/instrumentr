
#' @export
trace_packages <- function(object, package_names, ...) {
    UseMethod("trace_packages")
}

#' @export
trace_packages.lightr_context <- function(object, package_names, ...) {

    is_vector_character(package_names)

    for(package_name in package_names) {
        .Call(C_context_trace_package, object, package_name)
    }
}