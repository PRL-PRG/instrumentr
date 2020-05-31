
get_environment <- function(context, ...) {
    UseMethod("get_environment")
}

get_environment.lightr_context <- function(context, ...) {
    .Call(C_context_get_environment, context)
}
