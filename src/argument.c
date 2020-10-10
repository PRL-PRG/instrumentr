#include <instrumentr/argument.h>
#include "argument_internals.h"
#include <instrumentr/object.h>
#include <instrumentr/conversion.h>
#include <instrumentr/log.h>
#include <instrumentr/string.h>
#include <instrumentr/memory.h>
#include "object_internals.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_argument_impl_t {
    instrumentr_object_impl_t object;
    const char* name;
    SEXP r_promise;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_argument_finalize(instrumentr_object_t object) {
    instrumentr_argument_t argument = (instrumentr_argument_t)(object);

    free(argument->name);

    instrumentr_release_sexp(argument->r_promise);
    argument->r_promise = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t instrumentr_argument_create(const char* name,
                                                   SEXP r_promise) {
    if (TYPEOF(r_promise) != PROMSXP) {
        instrumentr_raise_error(
            "attempt to create argument object from a non promise");
    }

    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_argument_impl_t),
                                  INSTRUMENTR_ARGUMENT,
                                  instrumentr_argument_finalize);

    instrumentr_argument_t argument = (instrumentr_argument_t)(object);

    argument->name = duplicate_name;

    instrumentr_acquire_sexp(r_promise);
    argument->r_promise = r_promise;

    return argument;
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
        instrumentr_raise_error("argument does not have a name");
    }
}

SEXP r_instrumentr_argument_get_name(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    const char* name = instrumentr_argument_get_name(argument);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * r_promise
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_evaluated(instrumentr_argument_t argument) {
    return PRVALUE(argument->r_promise) != R_UnboundValue;
}

SEXP r_instrumentr_argument_is_evaluated(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    int result = instrumentr_argument_is_evaluated(argument);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_argument_get_promise(instrumentr_argument_t argument) {
    return argument->r_promise;
}

SEXP r_instrumentr_argument_get_promise(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    return instrumentr_argument_get_promise(argument);
}

/* accessor  */
SEXP instrumentr_argument_get_expression(instrumentr_argument_t argument) {
    return PREXPR(argument->r_promise);
}

SEXP r_instrumentr_argument_get_expression(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    return instrumentr_argument_get_expression(argument);
}

/* accessor  */
SEXP instrumentr_argument_get_value(instrumentr_argument_t argument) {
    if (instrumentr_argument_is_evaluated(argument)) {
        return PRVALUE(argument->r_promise);
    } else {
        instrumentr_raise_error("argument is not evaluated");
    }
}

SEXP r_instrumentr_argument_get_value(SEXP r_argument) {
    instrumentr_argument_t argument = instrumentr_argument_unwrap(r_argument);
    return instrumentr_argument_get_value(argument);
}
