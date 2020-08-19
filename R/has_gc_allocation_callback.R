
#' @export
has_gc_allocation_callback <- function(object, ...) {
    UseMethod("has_gc_allocation_callback")
}

#' @export
has_gc_allocation_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_gc_allocation_callback, object)
}
