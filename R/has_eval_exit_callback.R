
#' @export
has_eval_exit_callback <- function(object, ...) {
    UseMethod("has_eval_exit_callback")
}

#' @export
has_eval_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_eval_exit_callback, object)
}
