
get_initializer <- function(context, ...) {
    UseMethod("get_initializer")
}

get_initializer.lightr_context <- function(context, ...) {
    .Call(C_context_get_initializer, context)
}
