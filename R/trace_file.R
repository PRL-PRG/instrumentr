
#' @export
trace_file <- function(tracer, file, environment = .GlobalEnv, ...) {
    UseMethod("trace_file")
}

#' @export
trace_file.instrumentr_tracer <- function(tracer, file, environment = .GlobalEnv, ...) {
    code <- parse(file = file, keep.source = TRUE)

    trace_code(tracer, code, environment = environment, quote = FALSE)
}
