
#' @export
trace_file <- function(context, file, environment = .GlobalEnv, ...) {
    UseMethod("trace_file")
}

#' @export
trace_file.instrumentr_context <- function(context, file, environment = .GlobalEnv, ...) {
    code <- parse(file = file, keep.source = TRUE)

    trace_code(context, code, environment = environment, quote = FALSE)
}
