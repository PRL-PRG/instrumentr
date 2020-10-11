#include "instrumentr_internals.h"
#include "utilities.h"
#include "vec.h"

typedef vec_t(instrumentr_tracer_t) instrumentr_tracer_vector_t;

instrumentr_tracer_vector_t tracers;

const char* get_commit_hash() {
    return GIT_COMMIT_HASH;
}

SEXP r_instrumentr_get_commit_hash() {
    return instrumentr_c_string_to_r_character(instrumentr_get_commit_hash());
}

void instrumentr_add_tracer(instrumentr_tracer_t tracer) {
    vec_push(&tracers, tracer);
}

void instrumentr_remove_tracer(instrumentr_tracer_t tracer) {
    vec_remove(&tracers, tracer)
}

int instrumentr_is_tracing_enabled() {
    return !(tracers.length == 0) &&
           instrumentr_tracer_is_tracing_enabled(vec_last(&tracers));
}

SEXP r_instrumentr_is_tracing_enabled() {
    int result = instrumentr_is_tracing_enabled();
    return instrumentr_c_int_to_r_logical(result);
}

void instrumentr_set_tracing_status(int tracing_status) {
    for (int index = 0; index < tracers.length; ++index) {
        instrumentr_tracer_set_tracing_status(tracers.data[index],
                                              tracing_status);
    }
}

void instrumentr_enable_tracing() {
    instrumentr_set_tracing_status(true);
}

SEXP r_instrumentr_enable_tracing() {
    instrumentr_enable_tracing();
    return R_NilValue;
}

void instrumentr_disable_tracing() {
    instrumentr_set_tracing_status(false);
}

SEXP r_instrumentr_disable_tracing() {
    instrumentr_disable_tracing();
    return R_NilValue;
}

void instrumentr_reinstate_tracing() {
    for (int index = 0; index < tracers.length; ++index) {
        instrumentr_tracer_reinstate(tracers.data[index]);
    }
}

SEXP r_instrumentr_reinstate_tracing() {
    instrumentr_reinstate_tracing();
    return R_NilValue;
}

void instrumentr_initialize(SEXP r_package_environment,
                            SEXP r_state_environment) {
    vec_init(&tracers);
    instrumentr_initialize_utilities(r_package_environment,
                                     r_state_environment);
}

SEXP r_instrumentr_initialize(SEXP r_package_environment,
                             SEXP r_state_environment) {
    instrumentr_initialize(r_package_environment, r_state_environment);
    return R_NilValue;
}

void instrumentr_finalize() {
    instrumentr_finalize_utilities();
    vec_deinit(&tracers);
}

SEXP r_instrumentr_finalize_instrumentr() {
    instrumentr_finalize();
    return R_NilValue;
}
