#include "callback.h"
#include "object.h"
#include "vec.h"
#include "interop.h"
#include "event.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef union {
    void* c;
    SEXP r;
} function_t;

struct instrumentr_callback_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_event_t event;
    function_t function;
    int has_r_function;
    int active;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_callback_finalize(instrumentr_object_t object) {
    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    if (instrumentr_callback_has_r_function(callback)) {
        instrumentr_sexp_release(callback->function.r);
    }
    callback->function.r = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_callback_t
instrumentr_callback_create(instrumentr_callback_t callback,
                            instrumentr_event_t event) {
    callback->event = event;

    callback->active = 0;

    return callback;
}

instrumentr_callback_t instrumentr_callback_create_from_r_function(instrumentr_state_t state,
                                                                   SEXP r_function,
                                                                   instrumentr_event_t event) {
    const char* name = instrumentr_event_to_string(event);

    int expected_parameter_count =
        instrumentr_event_get_parameter_count(event);

    int actual_parameter_count = Rf_length(FORMALS(r_function));

    if (actual_parameter_count != expected_parameter_count) {
        instrumentr_log_error("callback %s expects a %d parameter R function "
                              "but received a %d parameter function",
                              name,
                              expected_parameter_count,
                              actual_parameter_count);
    }

    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_callback_impl_t),
                                                 state,
                                                 INSTRUMENTR_CALLBACK,
                                                 instrumentr_callback_finalize,
                                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    instrumentr_sexp_acquire(r_function);
    callback->function.r = r_function;

    callback->has_r_function = 1;

    return instrumentr_callback_create(callback, event);
}

SEXP
r_instrumentr_callback_create_from_r_function(SEXP r_state,
                                              SEXP r_function,
                                              SEXP r_event) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    instrumentr_callback_t callback = instrumentr_callback_create_from_r_function(state, r_function, event);
    return instrumentr_callback_wrap(callback);
}

instrumentr_callback_t instrumentr_callback_create_from_c_function(instrumentr_state_t state,
                                                                   void* c_function,
                                                                   instrumentr_event_t event) {
    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_callback_impl_t),
                                                 state,
                                                 INSTRUMENTR_CALLBACK,
                                                 instrumentr_callback_finalize,
                                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    callback->function.c = c_function;

    callback->has_r_function = 0;

    return instrumentr_callback_create(callback, event);
}

SEXP r_instrumentr_callback_create_from_c_function(SEXP r_state,
                                                   SEXP r_c_function,
                                                   SEXP r_event) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    void* c_function = instrumentr_r_externalptr_to_c_pointer(r_c_function);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    instrumentr_callback_t callback =
        instrumentr_callback_create_from_c_function(state, c_function, event);
    return instrumentr_callback_wrap(callback);
}


/********************************************************************************
* interop
*******************************************************************************/

SEXP instrumentr_callback_wrap(instrumentr_callback_t callback) {
    return instrumentr_object_wrap((instrumentr_object_t)(callback));
}

instrumentr_callback_t instrumentr_callback_unwrap(SEXP r_callback) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_callback, INSTRUMENTR_CALLBACK);
    return (instrumentr_callback_t)(object);
}

/********************************************************************************
 * event
 *******************************************************************************/

/* accessor  */
instrumentr_event_t
instrumentr_callback_get_event(instrumentr_callback_t callback) {
    return callback->event;
}

SEXP r_instrumentr_callback_get_event(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    instrumentr_event_t event = instrumentr_callback_get_event(callback);
    return instrumentr_event_wrap(event);
}

instrumentr_event_t
instrumentr_callback_handles_event(instrumentr_callback_t callback, instrumentr_event_t event) {
    return instrumentr_callback_get_event(callback) == event;
}

SEXP
r_instrumentr_callback_handles_event(SEXP r_callback, SEXP r_event) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    instrumentr_event_t event = instrumentr_callback_get_event(callback);
    int result = instrumentr_callback_handles_event(callback, event);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_get_parameter_count(instrumentr_callback_t callback) {
    instrumentr_event_t event =
        instrumentr_callback_get_event(callback);
    return instrumentr_event_get_parameter_count(event);
}

SEXP r_instrumentr_callback_get_parameter_count(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int parameter_count = instrumentr_callback_get_parameter_count(callback);
    return instrumentr_c_int_to_r_integer(parameter_count);
}

/********************************************************************************
 * r_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_r_function(instrumentr_callback_t callback) {
    return callback->has_r_function;
}

SEXP r_instrumentr_callback_has_r_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_has_r_function(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_callback_get_r_function(instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        return callback->function.r;
    } else {
        instrumentr_event_t event = instrumentr_callback_get_event(callback);
        const char* event_str = instrumentr_event_to_string(event);
        instrumentr_log_error("R function not set for callback %s", event_str);
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_callback_get_r_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    SEXP r_function = instrumentr_callback_get_r_function(callback);
    return r_function;
}

/********************************************************************************
 * c_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_c_function(instrumentr_callback_t callback) {
    return !instrumentr_callback_has_r_function(callback);
}

SEXP r_instrumentr_callback_has_c_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_has_c_function(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
void* instrumentr_callback_get_c_function(instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        instrumentr_event_t event = instrumentr_callback_get_event(callback);
        const char* event_str = instrumentr_event_to_string(event);
        instrumentr_log_error("C function not set for callback %s", event_str);
        /* NOTE: not executed */
        return NULL;
    } else {
        return callback->function.c;
    }
}

SEXP r_instrumentr_callback_get_c_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    void* c_function = instrumentr_callback_get_c_function(callback);
    return instrumentr_c_pointer_to_r_externalptr(
        c_function, R_NilValue, R_NilValue, NULL);
}

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_is_active(instrumentr_callback_t callback) {
    return callback->active;
}

SEXP r_instrumentr_callback_is_active(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_is_active(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator  */
void instrumentr_callback_activate(instrumentr_callback_t callback) {
    callback->active = 1;
}

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback) {
    callback->active = 0;
}
