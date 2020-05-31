
set_finalizer <- function(context, finalizer, ...) {
    UseMethod("set_finalizer")
}

set_finalizer.lightr_context <- function(context, finalizer, ...) {
    stopifnot(is_closure(finalizer))

    .Call(C_context_set_finalizer, context, finalizer)

    invisible(NULL)
}
