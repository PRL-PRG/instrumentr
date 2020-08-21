
#' @export
has_eval_entry_callback <- function(object, ...) {
    UseMethod("has_eval_entry_callback")
}

#' @export
has_eval_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_eval_entry_callback, object)
}
