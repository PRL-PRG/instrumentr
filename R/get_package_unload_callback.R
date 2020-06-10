
#' @export
get_package_unload_callback <- function(context, ...) {
    UseMethod("get_package_unload_callback")
}

#' @export
get_package_unload_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_unload_callback, context)
}
