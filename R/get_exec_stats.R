
#' @export
get_exec_stats <- function(object, ...) {
    UseMethod("get_exec_stats")
}

#' @export
get_exec_stats.instrumentr_tracer <- function(object, ...) {
    .Call(C_instrumentr_tracer_get_exec_stats, object)
}

#' @export
get_exec_stats.instrumentr_callback <- function(object, ...) {
    .Call(C_instrumentr_callback_get_exec_stats, object)
}
