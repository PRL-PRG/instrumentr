
#' @export
set_application_load_callback <- function(context, application_load_callback, ...) { # nolint
    UseMethod("set_application_load_callback")
}

#' @export
set_application_load_callback.lightr_context <- function(context, application_load_callback, ...) {  # nolint

    stopifnot(is_invalid_value(application_load_callback) ||
              is_closure(application_load_callback) &&
              length(formals(application_load_callback)) >= 2)

    assign("application_load_callback", application_load_callback, envir=get_environment(context))

    .Call(C_context_set_application_load_callback, context, application_load_callback)

    invisible(NULL)
}
