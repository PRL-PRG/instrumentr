
#' @export
set_package_load_callback <- function(context, callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_load_callback(callback)) {
        callback <- create_package_load_callback(callback)
    }

    .Call(C_context_set_package_load_callback, context, callback)

    invisible(NULL)
}
