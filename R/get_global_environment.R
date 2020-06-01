
#' @export
get_global_environment <- function(object, ...) {
    UseMethod("get_global_environment")
}

#' @export
get_global_environment.lightr_application <- function(object, ...) {
    .Call(C_application_get_global_environment, object)
}
