
#' @export
get_traced_packages <- function(object, ...) {
    UseMethod("get_traced_packages")
}

#' @export
get_traced_packages.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_get_traced_packages, object)
}
