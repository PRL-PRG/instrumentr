
#' @export
set_variable_removal_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_removal_callback")
}

#' @export
set_variable_removal_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_variable_removal_callback(callback)) {
        callback <- create_variable_removal_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_removal, tracer, callback)

    invisible(NULL)
}
