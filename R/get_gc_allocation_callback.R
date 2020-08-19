
#' @export
get_gc_allocation_callback <- function(context, ...) {
    UseMethod("get_gc_allocation_callback")
}

#' @export
get_gc_allocation_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_gc_allocation_callback, context)
}
