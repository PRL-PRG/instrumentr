#include "argument.h"
#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"
#include "promise.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_argument_impl_t {
    struct instrumentr_object_impl_t object;
    const char* name;
    instrumentr_object_t promise_or_value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_argument_finalize(instrumentr_object_t object) {
    instrumentr_argument_t argument = (instrumentr_argument_t)(object);

    free((char*)(argument->name));


    /* argument is the primary owner of value but a secondary owner of promise */
    if(argument -> promise_or_value -> type == INSTRUMENTR_VALUE) {
        instrumentr_object_kill(argument->promise_or_value);
    }
    else {
        instrumentr_object_release(argument->promise_or_value);
    }

    argument->promise_or_value = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t
instrumentr_argument_create(instrumentr_state_t state,
                            const char* name,
                            instrumentr_object_t promise_or_value) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_argument_impl_t),
                                                 state,
                                                 INSTRUMENTR_ARGUMENT,
                                                 instrumentr_argument_finalize,
                                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_argument_t argument = (instrumentr_argument_t)(object);

    argument->name = duplicate_name;

    instrumentr_object_acquire(promise_or_value);
    argument->promise_or_value = promise_or_value;

    return argument;
}

instrumentr_argument_t
instrumentr_argument_create_from_promise(instrumentr_state_t state,
                                         const char* name,
                                         instrumentr_promise_t promise) {
    return instrumentr_argument_create(state, name, (instrumentr_object_t)promise);
}

instrumentr_argument_t
instrumentr_argument_create_from_value(instrumentr_state_t state,
                                       const char* name,
                                       instrumentr_value_t value) {
    return instrumentr_argument_create(state, name, (instrumentr_object_t) value);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_argument_wrap(instrumentr_argument_t argument) {
    return instrumentr_object_wrap((instrumentr_object_t)(argument));
}

instrumentr_argument_t instrumentr_argument_unwrap(SEXP r_argument) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_argument, INSTRUMENTR_ARGUMENT);
    return (instrumentr_argument_t)(object);
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_has_name(instrumentr_argument_t argument) {
    return argument->name != NULL;
}

SEXP r_instrumentr_argument_has_name(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    int result = instrumentr_argument_has_name(argument);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
const char* instrumentr_argument_get_name(instrumentr_argument_t argument) {
    if (instrumentr_argument_has_name(argument)) {
        return argument->name;
    } else {
        instrumentr_log_error("argument does not have a name");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_argument_get_name(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    const char* name = instrumentr_argument_get_name(argument);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * promise
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_promise(instrumentr_argument_t argument) {
    return argument->promise_or_value->type == INSTRUMENTR_PROMISE;
}

SEXP r_instrumentr_argument_is_promise(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    int result = instrumentr_argument_is_promise(argument);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_promise_t
instrumentr_argument_as_promise(instrumentr_argument_t argument) {
    if (instrumentr_argument_is_promise(argument)) {
        return (instrumentr_promise_t)(argument->promise_or_value);
    } else {
        instrumentr_log_error("argument is not a promise");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_argument_as_promise(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    instrumentr_promise_t promise = instrumentr_argument_as_promise(argument);
    return instrumentr_promise_wrap(promise);
}


/********************************************************************************
 * value
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_value(instrumentr_argument_t argument) {
    return argument->promise_or_value->type == INSTRUMENTR_VALUE;
}

SEXP r_instrumentr_argument_is_value(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    int result = instrumentr_argument_is_value(argument);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_value_t
instrumentr_argument_as_value(instrumentr_argument_t argument) {
    if (instrumentr_argument_is_value(argument)) {
        return (instrumentr_value_t)(argument->promise_or_value);
    } else {
        instrumentr_log_error("argument is not a value");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_argument_as_value(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    instrumentr_value_t value = instrumentr_argument_as_value(argument);
    return instrumentr_value_wrap(value);
}
