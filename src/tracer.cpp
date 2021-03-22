#include <instrumentr/Rincludes.h>
#include "object.h"
#include "tracer.h"
#include "callback.h"
#include "application.h"
#include "vec.h"
#include "interop.h"
#include "utilities.h"
#include "dyntrace.h"
#include "dyntrace.h"
#include "state.h"
#include "application.h"
#include "event.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_tracer_impl_t {
    struct instrumentr_object_impl_t object;
    dyntracer_t* dyntracer;

    instrumentr_state_t state;
    instrumentr_application_t application;
    SEXP r_environment;
    instrumentr_callback_t active_callback;
    vec_int_t status;

    struct {
        instrumentr_callback_t callback;
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

    if (tracer->state != NULL) {
        instrumentr_object_release(tracer->state);
        tracer->state = NULL;
    }

    if (tracer->application != NULL) {
        instrumentr_model_kill(tracer->application);
        tracer->application = NULL;
    }

    instrumentr_sexp_release(tracer->r_environment);
    tracer->r_environment = NULL;

    tracer->active_callback = NULL;

    vec_deinit(&tracer->status);

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        if (tracer->callbacks[i].callback != NULL) {
            instrumentr_object_release(tracer->callbacks[i].callback);
            tracer->callbacks[i].callback = NULL;
        }
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_tracer_t instrumentr_tracer_create() {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_tracer_impl_t),
                                  INSTRUMENTR_OBJECT_TYPE_TRACER,
                                  instrumentr_tracer_finalize);

    instrumentr_tracer_t tracer = (instrumentr_tracer_t)(object);

    dyntracer_t* dyntracer = instrumentr_dyntracer_create(tracer);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This creates a cyclic reference, tracer holds a reference to itself via
       dyntracer.
    */
    tracer->dyntracer = dyntracer;

    tracer->state = instrumentr_state_create();

    tracer->application = NULL;

    SEXP r_environment = create_environment(ScalarLogical(1), R_EmptyEnv);
    tracer->r_environment = r_environment;
    instrumentr_sexp_acquire(r_environment);

    tracer->active_callback = NULL;

    vec_init(&tracer->status);

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        tracer->callbacks[i].callback = NULL;
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
 * tracing
 *******************************************************************************/

void instrumentr_tracer_initialize_tracing(instrumentr_tracer_t tracer,
                                           const char* working_directory,
                                           SEXP r_code,
                                           SEXP r_environment) {
    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_environment(
            tracer->state, r_environment, 1);

    tracer->application = instrumentr_application_create(
        tracer->state, working_directory, r_code, environment);

    instrumentr_state_value_table_initialize(tracer->state);
}

SEXP instrumentr_tracer_finalize_tracing(instrumentr_tracer_t tracer) {
    instrumentr_model_kill(tracer->application);
    tracer->application = NULL;
    SEXP r_result = PROTECT(instrumentr_state_finalize_tracing(tracer->state));
    instrumentr_object_release(tracer->state);
    tracer->state = NULL;
    UNPROTECT(1);
    return r_result;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DEFINE_API(tracer, INSTRUMENTR_OBJECT_TYPE_TRACER)

/********************************************************************************
 * dyntracer
 *******************************************************************************/

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer) {
    return tracer->dyntracer;
}

/********************************************************************************
 * state
 *******************************************************************************/

/*  accessor  */
instrumentr_state_t instrumentr_tracer_get_state(instrumentr_tracer_t tracer) {
    return tracer->state;
}

SEXP r_instrumentr_tracer_get_state(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);
    return instrumentr_state_wrap(state);
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
        /* TODO modify the return value to return current tracer status */
        vec_pop(&tracer->status);
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
