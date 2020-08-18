
#' @export
set_package_detach_callback <- function(context, callback, ...) {
    UseMethod("set_package_detach_callback")
}

#' @export
set_package_detach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_detach_callback(callback)) {
        callback <- create_package_detach_callback(callback)
    }

    .Call(C_context_set_package_detach_callback, context, callback)

    invisible(NULL)
}
