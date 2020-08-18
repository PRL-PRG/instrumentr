
#' @export
set_variable_removal_callback <- function(context, callback, ...) {
    UseMethod("set_variable_removal_callback")
}

#' @export
set_variable_removal_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_variable_removal_callback(callback)) {
        callback <- create_variable_removal_callback(callback)
    }

    .Call(C_context_set_variable_removal_callback, context, callback)

    invisible(NULL)
}
