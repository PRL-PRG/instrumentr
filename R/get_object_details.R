
#' @export
get_object_details <- function(value, variable, envir = parent.frame(), peek = TRUE) {

    if(missing(variable)) {
        variable <- NULL
    }
    else {
        stopifnot(is_scalar_character(variable))

        if(!exists(variable, envir)) {
            stop(cat("variable", variable, "does not exist in environment"))
        }
    }

    if(missing(value)) {
        value <- NULL
    }

    .Call(C_instrumentr_get_object_details, value, variable, envir, peek)
}
