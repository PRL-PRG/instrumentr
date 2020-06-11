
#' @export
set_package_detach_callback <- function(context, package_detach_callback, ...) {
    UseMethod("set_package_detach_callback")
}

#' @export
set_package_detach_callback.instrumentr_context <- function(context, package_detach_callback, ...) { # nolint

    stopifnot(is_undefined(package_detach_callback) ||
              is_closure(package_detach_callback) &&
              has_parameters(package_detach_callback, 3))

    .Call(C_context_set_package_detach_callback,
          context,
          package_detach_callback)

    invisible(NULL)
}
