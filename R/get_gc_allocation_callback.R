
#' @export
get_gc_allocation_callback <- function(tracer, ...) {
    UseMethod("get_gc_allocation_callback")
}

#' @export
get_gc_allocation_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_gc_allocation, tracer)
}
