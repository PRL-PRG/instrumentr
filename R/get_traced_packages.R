
#' @export
get_traced_packages <- function(object, ...) {
    UseMethod("get_traced_packages")
}

#' @export
get_traced_packages.lightr_context <- function(object, ...) {
    .Call(C_context_get_traced_packages, object)
}
