
#' @export
trace_text <- function(tracer, text, environment = .GlobalEnv, ...) {
    UseMethod("trace_text")
}

#' @export
trace_text.instrumentr_tracer <- function(tracer, text, environment = .GlobalEnv, ...) {
    code <- parse(text = text, keep.source = TRUE)

    trace_code(tracer, code, environment = environment, quote = FALSE)
}
