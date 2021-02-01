
#' Print instrumentr object
#'
#' @param x Input instrument object.
#'
#' @return The input object
#' @examples
#'
#' print(create_tracer())
#'
#' @export
print.instrumentr_object <- function(x, ...) {
    cat(to_string(x), "\n")
}
