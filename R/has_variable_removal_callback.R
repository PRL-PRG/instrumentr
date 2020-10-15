
#' @export
has_variable_removal_callback <- function(object, ...) {
    UseMethod("has_variable_removal_callback")
}

#' @export
has_variable_removal_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_removal, object)
}
