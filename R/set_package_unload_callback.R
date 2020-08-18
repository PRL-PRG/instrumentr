
#' @export
set_package_unload_callback <- function(context, callback, ...) {
    UseMethod("set_package_unload_callback")
}

#' @export
set_package_unload_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_unload_callback(callback)) {
        callback <- create_package_unload_callback(callback)
    }

    .Call(C_context_set_package_unload_callback, context, callback)

    invisible(NULL)
}
