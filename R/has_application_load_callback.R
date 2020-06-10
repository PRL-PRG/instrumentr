
#' @export
has_application_load_callback <- function(object, ...) {
    UseMethod("has_application_load_callback")
}

#' @export
has_application_load_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_load_callback, object)
}
