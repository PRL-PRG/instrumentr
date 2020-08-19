
#' @export
set_gc_allocation_callback <- function(context, callback, ...) {
    UseMethod("set_gc_allocation_callback")
}

#' @export
set_gc_allocation_callback.instrumentr_context <- function(context, callback, ...) {  # nolint

    if (!is_instrumentr_gc_allocation_callback(callback)) {
        callback <- create_gc_allocation_callback(callback)
    }

    .Call(C_context_set_gc_allocation_callback, context, callback)

    invisible(NULL)
}
