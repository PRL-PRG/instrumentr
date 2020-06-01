
get_environment <- function(object, ...) {
    UseMethod("get_environment")
}

get_environment.lightr_context <- function(object, ...) {
    .Call(C_context_get_environment, object)
}

get_environment.lightr_package <- function(object, ...) {
    .Call(C_package_get_environment, object)
}
