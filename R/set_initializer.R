
set_initializer <- function(context, initializer, ...) {
    UseMethod("set_initializer")
}

set_initializer.lightr_context <- function(context, initializer, ...) {
    stopifnot(is_closure(initializer))

    .Call(C_context_set_initializer, context, initializer)

    invisible(NULL)
}
