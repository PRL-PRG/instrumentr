#ifndef INSTRUMENTR_INSTRUMENTR_INTERNALS_H
#define INSTRUMENTR_INSTRUMENTR_INTERNALS_H

#include <instrumentr/tracer.h>

const char* instrumentr_get_commit_hash();
SEXP r_instrumentr_get_commit_hash();

void instrumentr_add_tracer(instrumentr_tracer_t tracer);

void instrumentr_remove_tracer(instrumentr_tracer_t tracer);

void instrumentr_initialize_tracing(instrumentr_tracer_t tracer);
SEXP r_instrumentr_initialize_tracing(SEXP r_tracer);

void instrumentr_finalize_tracing(instrumentr_tracer_t tracer);
SEXP r_instrumentr_finalize_tracing(SEXP r_tracer);

int instrumentr_is_tracing_enabled();
SEXP r_instrumentr_is_tracing_enabled();

void instrumentr_set_tracing_status(int tracing_status);

void instrumentr_enable_tracing();
SEXP r_instrumentr_enable_tracing();

void instrumentr_disable_tracing();
SEXP r_instrumentr_disable_tracing();

void instrumentr_reinstate_tracing();
SEXP r_instrumentr_reinstate_tracing();

SEXP r_instrumentr_initialize(SEXP r_package_environment,
                              SEXP r_state_environment);

SEXP r_instrumentr_finalize();

#endif /* INSTRUMENTR_INSTRUMENTR_INTERNALS_H */
