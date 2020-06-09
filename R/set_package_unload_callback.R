
#' @export
set_package_unload_callback <- function(context, package_unload_callback, ...) {
    UseMethod("set_package_unload_callback")
}

#' @export
set_package_unload_callback.lightr_context <- function(context, package_unload_callback, ...) { # nolint

    stopifnot(is_undefined(package_unload_callback) ||
              is_closure(package_unload_callback) &&
              length(formals(package_unload_callback)) >= 3)

    .Call(C_context_set_package_unload_callback,
          context,
          package_unload_callback)

    invisible(NULL)
}
