
#' @export
with_tracing_disabled <- function(tracer, code) {

    .Call(C_instrumentr_tracer_disable, tracer)

    on.exit(.Call(C_instrumentr_tracer_reinstate, tracer))

    code
}
