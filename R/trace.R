
#' @export
is_tracing_enabled <- function() {
    .Call(C_lightr_is_tracing_enabled)
}

#' @export
disable_tracing <- function() {
    .Call(C_lightr_disable_tracing)
    invisible(NULL)
}

#' @export
enable_tracing <- function() {
    .Call(C_lightr_enable_tracing)
    invisible(NULL)
}

#' @export
reinstate_tracing <- function() {
    .Call(C_lightr_reinstate_tracing)
    invisible(NULL)
}

