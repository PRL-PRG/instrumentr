
#' @export
is_package_traced <- function(object, package_name, ...) {
    UseMethod("is_package_traced")
}

#' @export
is_package_traced.lightr_context <- function(object, package_name, ...) { # nolint
    stopifnot(is_scalar_character(package_name))

    .Call(C_context_is_package_traced, object, package_name)
}
