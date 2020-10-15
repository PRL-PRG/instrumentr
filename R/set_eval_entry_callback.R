
#' @export
set_eval_entry_callback <- function(tracer, callback, ...) {
    UseMethod("set_eval_entry_callback")
}

#' @export
set_eval_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_eval_entry_callback(callback)) {
        callback <- create_eval_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_eval_entry, tracer, callback)

    invisible(NULL)
}
