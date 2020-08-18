
#' @export
set_call_exit_callback <- function(context, callback, ...) {
    UseMethod("set_call_exit_callback")
}

#' @export
set_call_exit_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_call_exit_callback(callback)) {
        callback <- create_call_exit_callback(callback)
    }

    .Call(C_context_set_call_exit_callback, context, callback)

    invisible(NULL)
}
