
#' @export
set_function_detach_callback <- function(context, callback, ...) {
    UseMethod("set_function_detach_callback")
}

#' @export
set_function_detach_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_function_detach_callback(callback)) {
        callback <- create_function_detach_callback(callback)
    }

    .Call(C_context_set_function_detach_callback, context, callback)

    invisible(NULL)
}
