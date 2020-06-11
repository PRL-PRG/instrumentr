
#' @export
set_package_load_callback <- function(context, package_load_callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.instrumentr_context <- function(context, package_load_callback, ...) { # nolint

    stopifnot(is_undefined(package_load_callback) ||
              is_closure(package_load_callback) &&
              has_parameters(package_load_callback, 3))

    .Call(C_context_set_package_load_callback,
          context,
          package_load_callback)

    invisible(NULL)
}
