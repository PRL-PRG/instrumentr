
#' @export
get_name <- function(argument, ...) {
    UseMethod("get_name")
}

#' @export
get_name.lightr_argument <- function(argument, ...) {
    .Call(C_argument_get_name, argument)
}
