#include <instrumentr/parameter.h>
#include <instrumentr/argument_vector.h>
#include <instrumentr/conversion.h>
#include <instrumentr/logging.h>
#include <string.h>



/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_parameter_impl_t {
    instrumentr_object_impl_t object;
    const char* name;
    int position;
    instrumentr_argument_vector_t arguments;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_parameter_finalize(instrumentr_object_t object) {
    instrumentr_parameter_t parameter = (instrumentr_parameter_t)(object);

    free(parameter->name);
    instrumentr_object_decrement_reference(parameter->arguments);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(const char* name,
                                                     int position) {
    instrumentr_argument_vector_t arguments =
        instrumentr_argument_vector_create();

    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_parameter_impl_t),
                                  INSTRUMENTR_PARAMETER,
                                  instrumentr_parameter_finalize);

    instrumentr_parameter_t parameter = (instrumentr_parameter_t)(object);

    parameter->name = duplicate_name;
    parameter->position = position;
    parameter->arguments = arguments;

    return parameter;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_parameter_wrap(instrumentr_parameter_t parameter) {
    return instrumentr_object_wrap((instrumentr_object_t)(parameter));
}

instrumentr_parameter_t instrumentr_parameter_unwrap(SEXP r_parameter) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_parameter);
    if (instrumentr_object_is_parameter(object)) {
        return (instrumentr_parameter_t)(object);
    } else {
        /* TODO: add type of R object  */
        instrumentr_raise_error("attempt to unwrap parameter from an "
                                "instrumentr object of incorrect type");
    }
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
 * arguments
 *******************************************************************************/

/* accessor  */
argument_vector_t
instrumentr_parameter_get_arguments(instrumentr_parameter_t parameter) {
    return parameter->arguments;
}

SEXP r_instrumentr_parameter_get_arguments(SEXP r_parameter) {
    instrumentr_parameter_t parameter =
        instrumentr_parameter_unwrap(r_parameter);

    instrumentr_argument_vector_t arguments =
        instrumentr_parameter_get_arguments(parameter);

    return instrumentr_argument_vector_wrap(arguments);
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
