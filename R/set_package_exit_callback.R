
set_package_exit_callback <- function(context, package_exit_callback, ...) {
    UseMethod("set_package_exit_callback")
}

set_package_exit_callback.lightr_context <- function(context, package_exit_callback, ...) { # nolint

    stopifnot(is_invalid_value(package_exit_callback) ||
              is_closure(package_exit_callback) &&
              length(formals(package_exit_callback)) >= 3)

    assign("package_exit_callback", package_exit_callback, envir=get_environment(context))

    .Call(C_context_set_package_exit_callback,
          context,
          package_exit_callback)

    invisible(NULL)
}
