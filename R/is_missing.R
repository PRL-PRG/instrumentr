
#' @export
is_missing <- function(object, ...) {
    UseMethod("is_missing")
}

#' @export
is_missing.instrumentr_parameter <- function(object, ...) { # nolint
    .Call(C_instrumentr_parameter_is_missing, object)
}
