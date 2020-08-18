
#' @export
set_application_unload_callback <- function(context, callback, ...) { # nolint
    UseMethod("set_application_unload_callback")
}

#' @export
set_application_unload_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_application_unload_callback(callback)) {
        callback <- create_application_unload_callback(callback)
    }

    .Call(C_context_set_application_unload_callback, context, callback)

    invisible(NULL)
}
