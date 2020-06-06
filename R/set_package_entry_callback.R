
set_package_entry_callback <- function(context, package_entry_callback, ...) {
    UseMethod("set_package_entry_callback")
}

set_package_entry_callback.lightr_context <- function(context, package_entry_callback, ...) { # nolint

    stopifnot(is_invalid_value(package_entry_callback) ||
              is_closure(package_entry_callback) &&
              length(formals(package_entry_callback)) >= 3)

    assign("package_entry_callback", package_entry_callback, envir=get_environment(context))

    .Call(C_context_set_package_entry_callback,
          context,
          package_entry_callback)

    invisible(NULL)
}
