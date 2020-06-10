
#' @export
get_package_detach_callback <- function(context, ...) {
    UseMethod("get_package_detach_callback")
}

#' @export
get_package_detach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_detach_callback, context)
}
