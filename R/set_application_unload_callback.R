
#' @export
set_application_unload_callback <- function(context, application_unload_callback, ...) { # nolint
    UseMethod("set_application_unload_callback")
}

#' @export
set_application_unload_callback.instrumentr_context <- function(context, application_unload_callback, ...) {  # nolint

    stopifnot(is_undefined(application_unload_callback) ||
              is_closure(application_unload_callback) &&
              has_parameters(application_unload_callback, 2))

    .Call(C_context_set_application_unload_callback, context, application_unload_callback)

    invisible(NULL)
}
