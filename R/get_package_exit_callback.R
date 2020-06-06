
#' @export
get_package_exit_callback <- function(context, ...) {
    UseMethod("get_package_exit_callback")
}

#' @export
get_package_exit_callback.lightr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_exit_callback, context)
}
