
#' @export
set_application_unload_callback <- function(context, application_unload_callback, ...) { # nolint
    UseMethod("set_application_unload_callback")
}

#' @export
set_application_unload_callback.lightr_context <- function(context, application_unload_callback, ...) {  # nolint

    stopifnot(is_invalid_value(application_unload_callback) ||
              is_closure(application_unload_callback) &&
              length(formals(application_unload_callback)) >= 2)

    assign("application_unload_callback", application_unload_callback, envir=get_environment(context))

    .Call(C_context_set_application_unload_callback, context, application_unload_callback)

    invisible(NULL)
}
