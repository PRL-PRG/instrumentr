#include "instrumentr_internals.h"
#include "utilities.h"
#include "vec.h"
#include "interop.h"
#include "tracer.h"
#include "object.h"
#include "object_type.h"
#include "model_type.h"

typedef vec_t(instrumentr_tracer_t) instrumentr_tracer_vector_t;

instrumentr_tracer_vector_t tracers;

const char* instrumentr_get_commit_hash() {
    return GIT_COMMIT_HASH;
}

SEXP r_instrumentr_get_commit_hash() {
    return instrumentr_c_string_to_r_character(instrumentr_get_commit_hash());
}

void instrumentr_add_tracer(instrumentr_tracer_t tracer) {
    vec_push(&tracers, tracer);
}

void instrumentr_remove_tracer(instrumentr_tracer_t tracer) {
    vec_remove(&tracers, tracer);
}

instrumentr_tracer_t instrumentr_get_tracer(int index) {
    if (index >= tracers.length) {
        instrumentr_log_error("attempt to access tracer out of bounds");
        return NULL;
    } else {
        return tracers.data[index];
    }
}

int instrumentr_get_tracer_count() {
    return tracers.length;
}

void instrumentr_initialize_tracing(instrumentr_tracer_t tracer) {
    instrumentr_add_tracer(tracer);
}

SEXP r_instrumentr_initialize_tracing(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_initialize_tracing(tracer);
    return R_NilValue;
}

void instrumentr_finalize_tracing(instrumentr_tracer_t tracer) {
    instrumentr_remove_tracer(tracer);
    instrumentr_tracer_reset(tracer);
}

SEXP r_instrumentr_finalize_tracing(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_finalize_tracing(tracer);
    return R_NilValue;
}

int instrumentr_is_tracing_enabled() {
    return !(tracers.length == 0) &&
           instrumentr_tracer_is_enabled(vec_last(&tracers));
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
    instrumentr_set_tracing_status(1);
}

SEXP r_instrumentr_enable_tracing() {
    instrumentr_enable_tracing();
    return R_NilValue;
}

void instrumentr_disable_tracing() {
    instrumentr_set_tracing_status(0);
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
    instrumentr_object_type_initialize();
    instrumentr_model_type_initialize();
}

SEXP r_instrumentr_initialize(SEXP r_package_environment,
                              SEXP r_state_environment) {
    instrumentr_initialize(r_package_environment, r_state_environment);
    return R_NilValue;
}

void instrumentr_finalize() {
    instrumentr_finalize_utilities();
    vec_deinit(&tracers);
    instrumentr_object_type_finalize();
    instrumentr_model_type_finalize();
}

SEXP r_instrumentr_finalize() {
    instrumentr_finalize();
    return R_NilValue;
}
