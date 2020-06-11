
#' @export
get_functions <- function(object, ...) {
    UseMethod("get_functions")
}

#' @export
get_functions.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_package_get_functions, object)
}
