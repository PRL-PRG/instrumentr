
#' @export
get_traced_packages <- function(object, ...) {
    UseMethod("get_traced_packages")
}

#' @export
get_traced_packages.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_get_traced_packages, object)
}
