
#' @export
is_evaluated <- function(argument, ...) {
    UseMethod("is_evaluated")
}

#' @export
is_evaluated.instrumentr_argument <- function(argument, ...) { # nolint
    .Call(C_instrumentr_argument_is_evaluated, argument)
}
