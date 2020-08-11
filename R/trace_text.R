
#' @export
trace_text <- function(context, text, environment = .GlobalEnv, ...) {
    UseMethod("trace_text")
}

#' @export
trace_text.instrumentr_context <- function(context, text, environment = .GlobalEnv, ...) {
    code <- parse(text = text, keep.source = TRUE)

    trace_code(context, code, environment = environment, quote = FALSE)
}
