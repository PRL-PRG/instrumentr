#include <instrumentr/parameter.h>
#include "parameter_internals.h"
#include <instrumentr/conversion.h>
#include <instrumentr/log.h>
#include <string.h>
#include "object_internals.h"
#include "vec.h"

typedef vec_t(instrumentr_argument_t) instrumentr_argument_vector_t;

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_parameter_impl_t {
    instrumentr_object_impl_t object;
    const char* name;
    int position;
    SEXP r_default_argument;
    instrumentr_argument_vector_t arguments;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_parameter_finalize(instrumentr_object_t object) {
    instrumentr_parameter_t parameter = (instrumentr_parameter_t)(object);

    free(parameter->name);

    instrumentr_sexp_release(parameter->r_default_argument);

    int count = parameter->arguments.length;
    instrumentr_argument_t arguments[] = parameter->arguments.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_argument_t argument = arguments[i];
        instrumentr_object_decrement_reference(argument);
    }

    vec_deinit(&parameter->arguments);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(const char* name,
                                                     int position,
                                                     SEXP r_default_argument) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_parameter_impl_t),
                                  INSTRUMENTR_PARAMETER,
                                  instrumentr_parameter_finalize);

    instrumentr_parameter_t parameter = (instrumentr_parameter_t)(object);

    parameter->name = duplicate_name;
    parameter->position = position;

    parameter->r_default_argument = r_default_argument;
    instrumentr_acquire_sexp(r_default_argument);

    vec_init(&parameter->arguments);

    return parameter;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_parameter_wrap(instrumentr_parameter_t parameter) {
    return instrumentr_object_wrap((instrumentr_object_t)(parameter));
}

instrumentr_parameter_t instrumentr_parameter_unwrap(SEXP r_parameter) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_parameter, INSTRUMENTR_PARAMETER);
    return (instrumentr_parameter_t)(object);
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_parameter_get_name(instrumentr_parameter_t parameter) {
    return parameter->name;
}
SEXP r_instrumentr_parameter_get_name(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    const char* name = instrumentr_parameter_get_name(parameter);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * position
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_get_position(instrumentr_parameter_t parameter) {
    return parameter->position;
}

SEXP r_instrumentr_parameter_get_position(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    int result = instrumentr_parameter_get_position(parameter);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * r_default_argument
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_has_default_argument(
    instrumentr_parameter_t parameter) {
    return parameter->r_default_argument != NULL;
}

SEXP r_instrumentr_parameter_has_default_argument(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    int result = instrumentr_parameter_has_default_argument(parameter);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_parameter_get_default_argument(
    instrumentr_parameter_t parameter) {
    if (instrumentr_parameter_has_default_argument(parameter)) {
        return parameter->r_default_argument;
    } else {
        instrumentr_raise_error("parameter does not have a default argument");
    }
}

SEXP r_instrumentr_parameter_get_default_argument(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    return instrumentr_parameter_get_default_argument(parameter);
}

/********************************************************************************
 * arguments
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_get_argument_count(
    instrumentr_parameter_t parameter) {
    return parameter->arguments.length;
}

SEXP r_instrumentr_parameter_get_argument_count(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    int count = instrumentr_parameter_get_argument_count(parameter);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
int instrumentr_parameter_is_missing(instrumentr_parameter_t parameter) {
    instrumentr_argument_vector_t arguments =
        instrumentr_parameter_get_arguments(parameter);
    return instrumentr_argument_vector_is_empty(arguments);
}

SEXP r_instrumentr_parameter_is_missing(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    int result = instrumentr_parameter_is_missing(parameter);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
int instrumentr_parameter_is_vararg(instrumentr_parameter_t parameter) {
    const char* name = instrumentr_parameter_get_name(parameter);
    return strncmp(name, "...") == 0;
}

SEXP r_instrumentr_parameter_is_vararg(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    int result = instrumentr_parameter_is_vararg(parameter);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_argument_t instrumentr_parameter_get_argument_by_position(
    instrumentr_parameter_t parameter,
    int position) {
    int count = instrumentr_parameter_get_argument_count(parameter);
    if (position < count && position >= 0) {
        return parameter->arguments.data[position];
    } else {
        instrumentr_raise_error(
            "attempt to access %d argument of a parameter with %d arguments",
            position,
            count);
    }
}

SEXP r_instrumentr_parameter_get_argument_by_position(SEXP r_parameter,
                                                      SEXP r_position) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_argument_t argument =
        instrumentr_parameter_get_argument_by_position(parameter, position);
    return instrumentr_argument_wrap(argument);
}

/* accessor  */
instrumentr_argument_t
instrumentr_parameter_get_argument_by_name(instrumentr_parameter_t parameter,
                                           const char* name) {
    int count = instrumentr_parameter_get_argument_count(parameter);
    instrumentr_argument_t arguments[] = parameter->arguments.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_argument_t argument = arguments[i];
        const char* actual_name = instrumentr_argument_get_name(argument);
        if (actual_name != NULL && strcmp(actual_name, name) == 0) {
            return instrumentr_argument_wrap(argument);
        }
    }

    instrumentr_raise_errro(
        "argument with name '%s' does not exist for this parameter", name);
}

SEXP r_instrumentr_parameter_get_argument_by_name(SEXP r_parameter,
                                                  SEXP r_name) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_argument_t argument =
        instrumentr_parameter_get_argument_by_name(parameter, name);
    return instrumentr_argument_wrap(argument);
}

/* accessor */
SEXP r_instrumentr_parameter_get_arguments(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);
    instrumentr_argument_t arguments[] = parameter->arguments.data;
    int count = instrumentr_parameter_get_argument_count(parameter);

    SEXP r_arguments = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_argument_t argument = arguments[i];
        const char* name = instrumentr_argument_get_name(argument);
        SET_VECTOR_ELT(r_arguments, i, instrumentr_argument_wrap(argument));
        SET_STRING_ELT(r_names, i, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_arguments, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_arguments;
}

/* mutator  */
void instrumentr_parameter_append_argument(instrumentr_parameter_t parameter,
                                           instrumentr_argument_t argument) {
    vec_push(&parameter->arguments, argument);
    instrumentr_object_increment_reference(argument);
}
