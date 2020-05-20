
#' @export
get_package_name <- function(call, ...) {
    UseMethod("get_package_name")
}

#' @export
get_package_name.lightr_call <- function(call, ...) {
    .Call(C_call_get_package_name, call)
}
