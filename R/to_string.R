
expr_to_string <- function(expr, n) {
    indentation <- paste(rep(" ", n - 1), sep="", collapse="")
    expr_strs <- deparse(expr)
    paste(expr_strs[1],
          paste(indentation, expr_strs[2:length(expr_strs)], collapse = "\n"),
          sep = "\n")
}

logical_to_string <- function(object) {
    c("FALSE", "TRUE")[object[1] + 1]
}

#' @export
to_string.default <- function(object, ...) {
    sprintf("<%s: %s>", typeof(object), get_object_details(object)$address)
}

#' @export
to_string.NULL <- function(object, ...) {
    "<null>"
}

#' @export
to_string.call <- function(object, ...) {
    paste(deparse(object), collapse = " ", sep = " ")
}





