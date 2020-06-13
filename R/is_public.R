
#' @export
is_public <- function(object, ...) {
    UseMethod("is_public")
}

#' @export
is_public.instrumentr_function <- function(object, ...) {
    .Call(C_function_is_public, object)
}
