
#' @export
get_package_load_callback <- function(context, ...) {
    UseMethod("get_package_load_callback")
}

#' @export
get_package_load_callback.lightr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_load_callback, context)
}

