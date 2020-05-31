

#' @export
set_context <- function(context, ...) {
    UseMethod("set_context")
}


#' @export
set_context.lightr_context <- function(context, ...) {
    .Call(C_lightr_set_context, context)
}
