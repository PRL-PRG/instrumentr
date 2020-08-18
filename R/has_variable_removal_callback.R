
#' @export
has_variable_removal_callback <- function(object, ...) {
    UseMethod("has_variable_removal_callback")
}

#' @export
has_variable_removal_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_removal_callback, object)
}
