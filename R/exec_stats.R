#' API for exec_stats objects
#'
#' @description
#' TODO
#'
#' @name exec_stats
NULL

#' @export
#' @rdname exec_stats
get_minimum_time.instrumentr_exec_stats <- function(exec_stats, event) {
    .Call(C_instrumentr_exec_stats_get_minimum_time, exec_stats, event)
}

#' @export
#' @rdname exec_stats
get_maximum_time.instrumentr_exec_stats <- function(exec_stats) {
    .Call(C_instrumentr_exec_stats_get_maximum_time, exec_stats, event)
}

#' @export
#' @rdname exec_stats
get_average_time.instrumentr_exec_stats <- function(exec_stats) {
    .Call(C_instrumentr_exec_stats_get_average_time, exec_stats, event)
}

#' @export
#' @rdname exec_stats
get_total_time.instrumentr_exec_stats <- function(exec_stats) {
    .Call(C_instrumentr_exec_stats_get_total_time, exec_stats, event)
}

#' @export
#' @rdname exec_stats
get_execution_count.instrumentr_exec_stats <- function(exec_stats) {
    .Call(C_instrumentr_exec_stats_get_execution_count, exec_stats, event)
}

#' @export
#' @rdname exec_stats
as.data.frame.instrumentr_exec_stats <- function(x, row.names = NULL, optional = FALSE, ...) {
    ## TODO
    ##.Call(C_instrumentr_exec_stats_as_data_frame, x)
}
