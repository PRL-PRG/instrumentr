
#' @export
trace_packages <- function(object, package_names, ...) {
    UseMethod("trace_packages")
}

#' @export
trace_packages.instrumentr_tracer <- function(object, package_names, ...) { # nolint

    is_vector_character(package_names)

    for (package_name in package_names) {
        .Call(C_instrumentr_tracer_trace_package, object, package_name)
    }
}
