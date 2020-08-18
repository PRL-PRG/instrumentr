
#' @export
set_application_attach_callback <- function(context, callback, ...) { # nolint
    UseMethod("set_application_attach_callback")
}

#' @export
set_application_attach_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_application_attach_callback(callback)) {
        callback <- create_application_attach_callback(callback)
    }

    .Call(C_context_set_application_attach_callback, context, callback)

    invisible(NULL)
}
