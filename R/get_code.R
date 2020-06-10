
#' @export
get_code <- function(object, ...) {
    UseMethod("get_code")
}

#' @export
get_code.instrumentr_application <- function(object, ...) {
    .Call(C_application_get_code, object)
}
