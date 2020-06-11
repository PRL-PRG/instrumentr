
#' @export
has_application_attach_callback <- function(object, ...) { # nolint
    UseMethod("has_application_attach_callback")
}

#' @export
has_application_attach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_attach_callback, object)
}
