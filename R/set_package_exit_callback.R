
set_package_exit_callback <- function(context, package_exit_callback, ...) {
    UseMethod("set_package_exit_callback")
}

set_package_exit_callback.lightr_context <- function(context, package_exit_callback, ...) {

    stopifnot(is_null(package_exit_callback) ||
              is_closure(package_exit_callback) &&
              length(formals(package_exit_callback)) >= 3)

    .Call(C_context_set_package_exit_callback,
          context,
          package_exit_callback)

    invisible(NULL)
}

