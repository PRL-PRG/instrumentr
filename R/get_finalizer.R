
get_finalizer <- function(context, ...) {
    UseMethod("get_finalizer")
}

get_finalizer.lightr_context <- function(context, ...) {
    .Call(C_context_get_finalizer, context)
}
