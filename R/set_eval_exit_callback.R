
#' @export
set_eval_exit_callback <- function(context, callback, ...) {
    UseMethod("set_eval_exit_callback")
}

#' @export
set_eval_exit_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_eval_exit_callback(callback)) {
        callback <- create_eval_exit_callback(callback)
    }

    .Call(C_context_set_eval_exit_callback, context, callback)

    invisible(NULL)
}
