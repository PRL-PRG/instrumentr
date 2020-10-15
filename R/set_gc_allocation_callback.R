
#' @export
set_gc_allocation_callback <- function(tracer, callback, ...) {
    UseMethod("set_gc_allocation_callback")
}

#' @export
set_gc_allocation_callback.instrumentr_tracer <- function(tracer, callback, ...) {  # nolint

    if (!is_instrumentr_gc_allocation_callback(callback)) {
        callback <- create_gc_allocation_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_gc_allocation, tracer, callback)

    invisible(NULL)
}
