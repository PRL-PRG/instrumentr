
#' @export
with_tracing_disabled <- function(code) {

    .Call(C_instrumentr_disable_tracing)

    on.exit(.Call(C_instrumentr_reinstate_tracing))

    code
}
