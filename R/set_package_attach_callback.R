
#' @export
set_package_attach_callback <- function(context, package_attach_callback, ...) {
    UseMethod("set_package_attach_callback")
}

#' @export
set_package_attach_callback.lightr_context <- function(context, package_attach_callback, ...) { # nolint

    stopifnot(is_undefined(package_attach_callback) ||
              is_closure(package_attach_callback) &&
              length(formals(package_attach_callback)) >= 3)

    .Call(C_context_set_package_attach_callback,
          context,
          package_attach_callback)

    invisible(NULL)
}
