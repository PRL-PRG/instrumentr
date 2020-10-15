
#' @export
get_namespace <- function(object, ...) {
    UseMethod("get_namespace")
}

#' @export
get_namespace.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_namespace, object)
}
