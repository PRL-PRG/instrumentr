
#' @export
get_environment <- function(object, ...) {
    UseMethod("get_environment")
}

#' @export
get_environment.instrumentr_context <- function(object, ...) {
    .Call(C_context_get_environment, object)
}

#' @export
get_environment.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_application_get_environment, object)
}

#' @export
get_environment.instrumentr_package <- function(object, ...) {
    .Call(C_package_get_environment, object)
}

#' @export
get_environment.instrumentr_call <- function(object, ...) {
    .Call(C_call_get_environment, object)
}
