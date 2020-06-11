
#' @export
set_application_attach_callback <- function(context, application_attach_callback, ...) { # nolint
    UseMethod("set_application_attach_callback")
}

#' @export
set_application_attach_callback.instrumentr_context <- function(context, application_attach_callback, ...) {  # nolint

    stopifnot(is_undefined(application_attach_callback) ||
              is_closure(application_attach_callback) &&
              has_parameters(application_attach_callback, 2))

    .Call(C_context_set_application_attach_callback, context, application_attach_callback)

    invisible(NULL)
}
