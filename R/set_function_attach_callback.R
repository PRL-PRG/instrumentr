
#' @export
set_function_attach_callback <- function(context, callback, ...) {
    UseMethod("set_function_attach_callback")
}

#' @export
set_function_attach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_function_attach_callback(callback)) {
        callback <- create_function_attach_callback(callback)
    }

    .Call(C_context_set_function_attach_callback, context, callback)

    invisible(NULL)
}
