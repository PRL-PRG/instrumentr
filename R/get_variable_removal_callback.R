
#' @export
get_variable_removal_callback <- function(context, ...) {
    UseMethod("get_variable_removal_callback")
}

#' @export
get_variable_removal_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_removal_callback, context)
}
