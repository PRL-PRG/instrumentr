
#' @export
has_application_unload_callback <- function(object, ...) { # nolint
    UseMethod("has_application_unload_callback")
}

#' @export
has_application_unload_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_unload_callback, object)
}
