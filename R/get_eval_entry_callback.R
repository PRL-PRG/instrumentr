
#' @export
get_eval_entry_callback <- function(context, ...) {
    UseMethod("get_eval_entry_callback")
}

#' @export
get_eval_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_eval_entry_callback, context)
}
