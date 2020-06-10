
#' @export
create_call <- function(func, call, environment) {

    stopifnot(is_instrumentr_function(func))
    stopifnot(is_language(call))
    stopifnot(is_environment(environment))

    .Call(C_call_create_call, func, call, environment)
}
