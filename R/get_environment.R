
#' @export
get_environment <- function(object, ...) {
    UseMethod("get_environment")
}

#' @export
get_environment.lightr_context <- function(object, ...) {
    .Call(C_context_get_environment, object)
}

#' @export
get_environment.lightr_application <- function(object, ...) {
    .Call(C_application_get_environment, object)
}

#' @export
get_environment.lightr_package <- function(object, ...) {
    .Call(C_package_get_environment, object)
}

#' @export
get_environment.lightr_call <- function(object, ...) {
    .Call(C_call_get_environment, object)
}
