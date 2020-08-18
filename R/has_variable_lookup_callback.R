
#' @export
has_variable_lookup_callback <- function(object, ...) {
    UseMethod("has_variable_lookup_callback")
}

#' @export
has_variable_lookup_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_lookup_callback, object)
}
