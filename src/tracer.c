#include <instrumentr/Rincludes.h>
#include "object.h"
#include "tracer.h"
#include "callback.h"
#include "application.h"
#include "vec.h"
#include "interop.h"
#include "utilities.h"
#include "dyntrace.h"
#include "dyntracer.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_tracer_impl_t {
    struct instrumentr_object_impl_t object;
    dyntracer_t* dyntracer;

    instrumentr_application_t application;
    SEXP r_environment;
    instrumentr_callback_t active_callback;
    vec_int_t status;

    instrumentr_exec_stats_t tracing_exec_stats;

    struct {
        instrumentr_callback_t callback;
        instrumentr_exec_stats_t exec_stats;
    } callbacks[INSTRUMENTR_EVENT_COUNT];
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_tracer_finalize(instrumentr_object_t object) {
    instrumentr_tracer_t tracer = (instrumentr_tracer_t)(object);

    instrumentr_dyntracer_destroy(tracer->dyntracer);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This creates a cyclic reference, tracer holds a reference to itself via
       dyntracer.
    */
    tracer->dyntracer = NULL;

    if (tracer->application != NULL) {
        instrumentr_object_release(tracer->application);
        tracer->application = NULL;
    }

    instrumentr_sexp_release(tracer->r_environment);
    tracer->r_environment = NULL;

    tracer->active_callback = NULL;

    vec_deinit(&tracer->status);

    instrumentr_exec_stats_destroy(tracer->tracing_exec_stats);
    tracer->tracing_exec_stats = NULL;

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        if (tracer->callbacks[i].callback != NULL) {
            instrumentr_object_release(tracer->callbacks[i].callback);
            tracer->callbacks[i].callback = NULL;
        }
        /* NOTE: exec_stats is created for all callbacks even if they are not
         * attached */
        instrumentr_exec_stats_destroy(tracer->callbacks[i].exec_stats);
        tracer->callbacks[i].exec_stats = NULL;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_tracer_t instrumentr_tracer_create() {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_tracer_impl_t),
                                  INSTRUMENTR_TRACER,
                                  instrumentr_tracer_finalize);

    instrumentr_tracer_t tracer = (instrumentr_tracer_t)(object);

    dyntracer_t* dyntracer = instrumentr_dyntracer_create(tracer);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This creates a cyclic reference, tracer holds a reference to itself via
       dyntracer.
    */
    tracer->dyntracer = dyntracer;

    tracer->application = NULL;

    SEXP r_environment = create_environment(ScalarLogical(1), R_EmptyEnv);
    tracer->r_environment = r_environment;
    instrumentr_sexp_acquire(r_environment);

    tracer->active_callback = NULL;

    vec_init(&tracer->status);

    tracer->tracing_exec_stats = instrumentr_exec_stats_create();
    instrumentr_object_acquire(tracer->tracing_exec_stats);

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        tracer->callbacks[i].callback = NULL;
        tracer->callbacks[i].exec_stats = instrumentr_exec_stats_create();
        instrumentr_object_acquire(tracer->callbacks[i].exec_stats);
    }

    return tracer;
}

SEXP r_instrumentr_tracer_create() {
    instrumentr_tracer_t tracer = instrumentr_tracer_create();
    SEXP r_tracer = instrumentr_tracer_wrap(tracer);
    instrumentr_object_release(tracer);
    return r_tracer;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_tracer_wrap(instrumentr_tracer_t tracer) {
    return instrumentr_object_wrap((instrumentr_object_t)(tracer));
}

instrumentr_tracer_t instrumentr_tracer_unwrap(SEXP r_tracer) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_tracer, INSTRUMENTR_TRACER);
    return (instrumentr_tracer_t)(object);
}

/********************************************************************************
 * dyntracer
 *******************************************************************************/

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer) {
    return tracer->dyntracer;
}

/********************************************************************************
 * application
 *******************************************************************************/

/*  accessor  */
instrumentr_application_t
instrumentr_tracer_get_application(instrumentr_tracer_t tracer) {
    return tracer->application;
}

SEXP r_instrumentr_tracer_get_application(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);
    return instrumentr_application_wrap(application);
}

/*  mutator  */
void instrumentr_tracer_set_application(instrumentr_tracer_t tracer,
                                        instrumentr_application_t application) {
    tracer->application = application;
    instrumentr_object_acquire(application);
}

/*  mutator  */
void instrumentr_tracer_remove_application(instrumentr_tracer_t tracer) {
    instrumentr_object_release(tracer->application);
    tracer->application = NULL;
}

/********************************************************************************
 * r_environment
 *******************************************************************************/

/*  accessor  */
SEXP instrumentr_tracer_get_environment(instrumentr_tracer_t tracer) {
    return tracer->r_environment;
}

SEXP r_instrumentr_tracer_get_environment(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    return instrumentr_tracer_get_environment(tracer);
}

/********************************************************************************
 * active_callback
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_active(instrumentr_tracer_t tracer) {
    return tracer->active_callback != NULL;
}

SEXP r_instrumentr_tracer_is_active(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    int result = instrumentr_tracer_is_active(tracer);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_callback_t
instrumentr_tracer_get_active_callback(instrumentr_tracer_t tracer) {
    return tracer->active_callback;
}

SEXP r_instrumentr_tracer_get_active_callback(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_callback_t callback =
        instrumentr_tracer_get_active_callback(tracer);
    return instrumentr_callback_wrap(callback);
}

/* mutator  */
void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback) {
    if (tracer->active_callback != NULL) {
        instrumentr_log_error(
            "attempt to set active callback when it is already set");
    } else {
        tracer->active_callback = callback;
        instrumentr_object_acquire(callback);
    }
}

/* mutator  */
void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer) {
    if (tracer->active_callback != NULL) {
        instrumentr_object_release(tracer->active_callback);
        tracer->active_callback = NULL;
    }
}

/********************************************************************************
 * status
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_enabled(instrumentr_tracer_t tracer) {
    return (tracer->status.length != 0) && vec_last(&tracer->status);
}

SEXP r_instrumentr_tracer_is_enabled(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    int result = instrumentr_tracer_is_enabled(tracer);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator  */
void instrumentr_tracer_enable(instrumentr_tracer_t tracer) {
    vec_push(&tracer->status, 1);
#ifdef USING_DYNTRACE
    dyntrace_enable();
#endif
}

SEXP r_instrumentr_tracer_enable(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_enable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_disable(instrumentr_tracer_t tracer) {
    vec_push(&tracer->status, 0);
#ifdef USING_DYNTRACE
    dyntrace_disable();
#endif
}

SEXP r_instrumentr_tracer_disable(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_disable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_reinstate(instrumentr_tracer_t tracer) {
    if (tracer->status.length != 0) {
        int result = vec_pop(&tracer->status);
    }
#ifdef USING_DYNTRACE
    if (instrumentr_tracer_is_enabled(tracer)) {
        dyntrace_enable();
    } else {
        dyntrace_disable();
    }
#endif
}

SEXP r_instrumentr_tracer_reinstate(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_reinstate(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status) {
    vec_push(&tracer->status, status);
}

/********************************************************************************
 * reset
 *******************************************************************************/

void instrumentr_tracer_reset(instrumentr_tracer_t tracer) {
    instrumentr_tracer_unset_active_callback(tracer);
    vec_clear(&tracer->status);
}

/********************************************************************************
 * callbacks
 *******************************************************************************/

void bind_callback(instrumentr_tracer_t tracer,
                   instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        instrumentr_event_t event = instrumentr_callback_get_event(callback);
        const char* name = instrumentr_event_to_string(event);
        SEXP r_name = Rf_install(name);
        Rf_defineVar(r_name,
                     instrumentr_callback_get_r_function(callback),
                     instrumentr_tracer_get_environment(tracer));
    }
}

void unbind_callback(instrumentr_tracer_t tracer,
                     instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        instrumentr_event_t event = instrumentr_callback_get_event(callback);
        const char* name = instrumentr_event_to_string(event);
        SEXP r_name = Rf_install(name);
        R_removeVarFromFrame(r_name,
                             instrumentr_tracer_get_environment(tracer));
    }
}

/* accessor */
int instrumentr_tracer_has_callback(instrumentr_tracer_t tracer,
                                    instrumentr_event_t event) {
    return tracer->callbacks[event].callback != NULL;
}

SEXP r_instrumentr_tracer_has_callback(SEXP r_tracer, SEXP r_event) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    int result = instrumentr_tracer_has_callback(tracer, event);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
instrumentr_callback_t
instrumentr_tracer_get_callback(instrumentr_tracer_t tracer,
                                instrumentr_event_t event) {
    if (instrumentr_tracer_has_callback(tracer, event)) {
        return tracer->callbacks[event].callback;
    } else {
        instrumentr_log_error("tracer does not have a %s callback",
                              instrumentr_event_to_string(event));
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_tracer_get_callback(SEXP r_tracer, SEXP r_event) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    instrumentr_callback_t callback =
        instrumentr_tracer_get_callback(tracer, event);
    return instrumentr_callback_wrap(callback);
}

/* accessor */
void instrumentr_tracer_set_callback(instrumentr_tracer_t tracer,
                                     instrumentr_callback_t callback) {
    instrumentr_event_t event = instrumentr_callback_get_event(callback);
    instrumentr_tracer_remove_callback(tracer, event);
    tracer->callbacks[event].callback = callback;
    instrumentr_object_acquire(callback);
    bind_callback(tracer, callback);
}

SEXP r_instrumentr_tracer_set_callback(SEXP r_tracer, SEXP r_callback) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    instrumentr_tracer_set_callback(tracer, callback);
    return R_NilValue;
}

/* mutator */
void instrumentr_tracer_remove_callback(instrumentr_tracer_t tracer,
                                        instrumentr_event_t event) {
    if (instrumentr_tracer_has_callback(tracer, event)) {
        instrumentr_callback_t callback = tracer->callbacks[event].callback;
        unbind_callback(tracer, callback);
        instrumentr_object_release(callback);
        tracer->callbacks[event].callback = NULL;
    }
}

SEXP r_instrumentr_tracer_remove_callback(SEXP r_tracer, SEXP r_event) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    instrumentr_tracer_remove_callback(tracer, event);
    return R_NilValue;
}

/********************************************************************************
 * exec_stats
 *******************************************************************************/

/* accessor */
instrumentr_exec_stats_t
instrumentr_tracer_get_tracing_exec_stats(instrumentr_tracer_t tracer) {
    return tracer->tracing_exec_stats;
}

/* accessor */
SEXP r_instrumentr_tracer_get_tracing_exec_stats(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_exec_stats_t exec_stats =
        instrumentr_tracer_get_tracing_exec_stats(tracer);

    return instrumentr_exec_stats_wrap(exec_stats);
}

instrumentr_exec_stats_t
instrumentr_tracer_get_event_exec_stats(instrumentr_tracer_t tracer,
                                           instrumentr_event_t event) {
    return tracer->callbacks[event].exec_stats;
}

SEXP r_instrumentr_tracer_get_event_exec_stats(SEXP r_tracer, SEXP r_event) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    instrumentr_exec_stats_t exec_stats =
        instrumentr_tracer_get_event_exec_stats(tracer, event);
    return instrumentr_exec_stats_wrap(exec_stats);
}

void assign_exec_stats_fields(instrumentr_exec_stats_t exec_stats,
                              const char* event_name,
                              int index,
                              SEXP r_callback,
                              SEXP r_count,
                              SEXP r_minimum,
                              SEXP r_maximum,
                              SEXP r_average,
                              SEXP r_total) {
    SET_STRING_ELT(r_callback, index, mkChar(event_name));

    int count = instrumentr_exec_stats_get_execution_count(exec_stats);
    INTEGER(r_count)[index] = count;

    double minimum = instrumentr_exec_stats_get_minimum_time(exec_stats);
    REAL(r_minimum)[index] = minimum;

    double maximum = instrumentr_exec_stats_get_maximum_time(exec_stats);
    REAL(r_maximum)[index] = maximum;

    double average = instrumentr_exec_stats_get_average_time(exec_stats);
    REAL(r_average)[index] = average;

    double total = instrumentr_exec_stats_get_total_time(exec_stats);
    REAL(r_total)[index] = total;
}

SEXP r_instrumentr_tracer_get_exec_stats(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    /* NOTE: there will always be 1 row corresponding to an end-to-end tracing
     * exec stats */
    int row_count = 1;

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        instrumentr_exec_stats_t exec_stats = tracer->callbacks[i].exec_stats;
        int count = instrumentr_exec_stats_get_execution_count(exec_stats);
        if (count != 0)
            ++row_count;
    }

    SEXP r_callback = PROTECT(allocVector(STRSXP, row_count));
    SEXP r_count = PROTECT(allocVector(INTSXP, row_count));
    SEXP r_minimum = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_maximum = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_average = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_total = PROTECT(allocVector(REALSXP, row_count));

    int index = 0;

    for (int event = 0; event < INSTRUMENTR_EVENT_COUNT; ++event) {
        instrumentr_exec_stats_t exec_stats =
            tracer->callbacks[event].exec_stats;
        int count = instrumentr_exec_stats_get_execution_count(exec_stats);

        if(count == 0) {
            continue;
        }

        assign_exec_stats_fields(
            exec_stats,
            instrumentr_event_to_string(index),
            index,
            r_callback,
            r_count,
            r_minimum,
            r_maximum,
            r_average,
            r_total);

        ++index;
    }

    instrumentr_exec_stats_t exec_stats =
        instrumentr_tracer_get_tracing_exec_stats(tracer);
    assign_exec_stats_fields(exec_stats,
                             "tracing",
                             index,
                             r_callback,
                             r_count,
                             r_minimum,
                             r_maximum,
                             r_average,
                             r_total);

    SEXP r_data_frame = PROTECT(instrumentr_create_data_frame(6,
                                                              "callback",
                                                              r_callback,
                                                              "execution_count",
                                                              r_count,
                                                              "minimum_time",
                                                              r_minimum,
                                                              "maximum_time",
                                                              r_maximum,
                                                              "average_time",
                                                              r_average,
                                                              "total_time",
                                                              r_total));

    UNPROTECT(7);

    return r_data_frame;
}
