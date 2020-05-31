
set_environment <- function(context, environment, ...) {
    UseMethod("set_environment")
}

set_environment.lightr_context <- function(context, environment, ...) {
    stopifnot(is_environment(environment))

    .Call(C_context_set_environment, context, environment)

    invisible(NULL)
}
