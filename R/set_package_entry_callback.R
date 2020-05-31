
set_package_entry_callback <- function(context, package_entry_callback, ...) {
    UseMethod("set_package_entry_callback")
}

set_package_entry_callback.lightr_context <- function(context, package_entry_callback, ...) {
    stopifnot(is_closure(package_entry_callback))

    .Call(C_context_set_package_entry_callback,
          context,
          package_entry_callback)

    invisible(NULL)
}

