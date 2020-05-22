
#' @export
get_application <- function(package, ...) {
    .Call(C_lightr_get_application)
}

