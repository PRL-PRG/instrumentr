
#' @export
with_tracing_enabled <- function(tracer, code) {

    on.exit(.Call(C_instrumentr_tracer_reinstate, tracer))

    .Call(C_instrumentr_tracer_enable, tracer)

    code
}
