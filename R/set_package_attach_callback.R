
#' @export
set_package_attach_callback <- function(context, package_attach_callback, ...) {
    UseMethod("set_package_attach_callback")
}

#' @export
set_package_attach_callback.instrumentr_context <- function(context, package_attach_callback, ...) { # nolint

    stopifnot(is_undefined(package_attach_callback) ||
              is_closure(package_attach_callback) &&
              has_parameters(package_attach_callback, 3))

    .Call(C_context_set_package_attach_callback,
          context,
          package_attach_callback)

    invisible(NULL)
}
