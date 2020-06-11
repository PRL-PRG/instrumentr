
#' @export
set_application_load_callback <- function(context, application_load_callback, ...) { # nolint
    UseMethod("set_application_load_callback")
}

#' @export
set_application_load_callback.instrumentr_context <- function(context, application_load_callback, ...) {  # nolint

    stopifnot(is_undefined(application_load_callback) ||
              is_closure(application_load_callback) &&
              has_parameters(application_load_callback, 2))

    .Call(C_context_set_application_load_callback, context, application_load_callback)

    invisible(NULL)
}
