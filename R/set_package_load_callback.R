
#' @export
set_package_load_callback <- function(context, package_load_callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.lightr_context <- function(context, package_load_callback, ...) { # nolint

    stopifnot(is_invalid_value(package_load_callback) ||
              is_closure(package_load_callback) &&
              length(formals(package_load_callback)) >= 3)

    .Call(C_context_set_package_load_callback,
          context,
          package_load_callback)

    invisible(NULL)
}
