
#' @export
get_package_attach_callback <- function(context, ...) {
    UseMethod("get_package_attach_callback")
}

#' @export
get_package_attach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_attach_callback, context)
}
