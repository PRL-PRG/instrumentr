
#' @export
with_tracing_enabled <- function(code) {

    .Call(C_instrumentr_enable_tracing)

    on.exit(.Call(C_instrumentr_reinstate_tracing))

    code
}
