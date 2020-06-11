
#' @export
get_application_unload_callback <- function(context, ...) { # nolint
    UseMethod("get_application_unload_callback")
}

#' @export
get_application_unload_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_application_unload_callback, context)
}
