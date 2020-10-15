
#' @export
set_variable_lookup_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_lookup_callback")
}

#' @export
set_variable_lookup_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_variable_lookup_callback(callback)) {
        callback <- create_variable_lookup_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_lookup, tracer, callback)

    invisible(NULL)
}
