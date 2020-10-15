
#' @export
has_gc_allocation_callback <- function(object, ...) {
    UseMethod("has_gc_allocation_callback")
}

#' @export
has_gc_allocation_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_gc_allocation, object)
}
