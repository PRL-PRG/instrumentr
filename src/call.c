#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "vec.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_parameter_t) instrumentr_parameter_vector_t;

struct instrumentr_call_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_function_t function;
    SEXP r_expression;
    SEXP r_environment;
    int frame_position;
    int active;
    SEXP r_result;
    instrumentr_parameter_vector_t parameters;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_finalize(instrumentr_object_t object) {
    instrumentr_call_t call = (instrumentr_call_t)(object);

    instrumentr_object_release(call->function);

    instrumentr_sexp_release(call->r_expression);
    instrumentr_sexp_release(call->r_environment);
    instrumentr_sexp_release(call->r_result);

    int count = call->parameters.length;
    instrumentr_parameter_t* parameters = call->parameters.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        instrumentr_object_release(parameter);
    }

    vec_deinit(&call->parameters);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_t instrumentr_call_create(instrumentr_function_t function,
                                           SEXP r_expression,
                                           SEXP r_environment,
                                           int frame_position) {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_call_impl_t),
                                  INSTRUMENTR_CALL,
                                  instrumentr_call_finalize);

    instrumentr_call_t call = (instrumentr_call_t)(object);

    call->function = function;
    instrumentr_object_acquire(call->function);

    call->r_expression = r_expression;
    instrumentr_sexp_acquire(call->r_expression);

    call->r_environment = r_environment;
    instrumentr_sexp_acquire(call->r_environment);

    call->frame_position = frame_position;

    vec_init(&call->parameters);

    /* TODO - handle calls to special and builtin */
    instrumentr_function_definition_t function_definition =
        instrumentr_function_get_definition(function);

    SEXP r_parameter_list = FORMALS(function_definition.sexp);

    for (int parameter_position = 0; r_parameter_list != R_NilValue;
         ++parameter_position, r_parameter_list = CDR(r_parameter_list)) {
        SEXP r_argument_name = TAG(r_parameter_list);

        SEXP r_default_argument = CAR(r_parameter_list);

        SEXP r_argument_value =
            Rf_findVarInFrame(r_environment, r_argument_name);

        instrumentr_parameter_t parameter = instrumentr_parameter_create(
            R_CHAR(PRINTNAME(r_argument_name)),
            parameter_position,
            r_default_argument == R_MissingArg ? NULL : r_default_argument);

        instrumentr_call_append_parameter(call, parameter);

        /* NOTE: parameter owned by call now */
        instrumentr_object_release(parameter);

        /* missing argument */
        if (r_argument_value == R_UnboundValue ||
            r_argument_value == R_MissingArg) {
            /* NOTE: do nothing */
        }
        /* ... parameter */
        else if (TYPEOF(r_argument_value) == DOTSXP) {
            vec_t(SEXP) dot_arguments;
            vec_t(SEXP) dot_names;

            vec_init(&dot_arguments);
            vec_init(&dot_names);

            for (SEXP r_dot_argument_pointer = r_argument_value;
                 r_dot_argument_pointer != R_NilValue;
                 r_dot_argument_pointer = CDR(r_dot_argument_pointer)) {
                SEXP r_dot_argument_name = TAG(r_dot_argument_pointer);

                SEXP r_dot_argument_value = CAR(r_dot_argument_pointer);

                if (TYPEOF(r_dot_argument_value) != PROMSXP) {
                    /*FIXME: this should be caller env!*/
                    r_dot_argument_value =
                        create_promise(r_dot_argument_value, r_environment);
                }

                vec_push(&dot_arguments, PROTECT(r_dot_argument_value));

                vec_push(&dot_names, r_dot_argument_name);
            }

            SEXP dots = PROTECT(R_NilValue);

            for (int i = dot_arguments.length - 1; i >= 0; --i) {
                SEXP r_dot_argument_value = dot_arguments.data[i];
                SEXP r_dot_argument_name = dot_names.data[i];

                const char* argument_name = NULL;

                if (r_dot_argument_name != R_NilValue &&
                    r_dot_argument_name != R_MissingArg &&
                    r_dot_argument_name != R_UnboundValue &&
                    TYPEOF(r_dot_argument_name) == SYMSXP) {
                    argument_name = CHAR(PRINTNAME(r_dot_argument_name));
                }

                /* this unprotects the cons cell created on the next line by the
                 * previous iteration. */
                UNPROTECT(1);

                dots = PROTECT(CONS(r_dot_argument_value, dots));

                SET_TAG(dots, r_dot_argument_name);

                instrumentr_argument_t argument = instrumentr_argument_create(
                    argument_name, r_dot_argument_value);

                instrumentr_parameter_append_argument(parameter, argument);

                /* NOTE: argument is owned by parameter now */
                instrumentr_object_release(argument);
            }

            SET_TYPEOF(dots, DOTSXP);

            Rf_setVar(R_DotsSymbol, dots, r_environment);

            UNPROTECT(1);

            UNPROTECT(dot_arguments.length);

            vec_deinit(&dot_arguments);
            vec_deinit(&dot_names);
        }
        /* non ... parameter  */
        else {
            /* value (promise not created in some cases)  */
            if (TYPEOF(r_argument_value) != PROMSXP) {
                SEXP r_argument_name_str =
                    PROTECT(mkString(CHAR(PRINTNAME(r_argument_name))));

                r_argument_value = delayed_assign(r_argument_name,
                                                  r_argument_name_str,
                                                  r_argument_value,
                                                  r_environment,
                                                  r_environment,
                                                  r_environment);
                UNPROTECT(1);
            }

            instrumentr_argument_t argument =
                instrumentr_argument_create(NULL, r_argument_value);

            instrumentr_parameter_append_argument(parameter, argument);

            /* NOTE: argument is owned by parameter now */
            instrumentr_object_release(argument);
        }
    }

    return call;
}

SEXP r_instrumentr_call_create(SEXP r_function,
                               SEXP r_expression,
                               SEXP r_environment,
                               SEXP r_frame_position) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int frame_position = instrumentr_r_integer_to_c_int(r_frame_position);

    /* TODO - handle calls to special and builtin */
    instrumentr_function_definition_t definition =
        instrumentr_function_get_definition(function);
    SEXP r_definition = definition.sexp;

    instrumentr_call_t call = instrumentr_call_create(
        function, r_expression, r_environment, frame_position);

    SEXP r_call = instrumentr_call_wrap(call);

    instrumentr_object_release(call);

    return r_call;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_call_wrap(instrumentr_call_t call) {
    return instrumentr_object_wrap((instrumentr_object_t)(call));
}

instrumentr_call_t instrumentr_call_unwrap(SEXP r_call) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_call, INSTRUMENTR_CALL);
    return (instrumentr_call_t)(object);
}

/********************************************************************************
 * function
 *******************************************************************************/

/* accessor  */
instrumentr_function_t instrumentr_call_get_function(instrumentr_call_t call) {
    return call->function;
}

SEXP r_instrumentr_call_get_function(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_function_t function = instrumentr_call_get_function(call);
    return instrumentr_function_wrap(function);
}

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_expression(instrumentr_call_t call) {
    return call->r_expression;
}

SEXP r_instrumentr_call_get_expression(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_expression(call);
}

/********************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_environment(instrumentr_call_t call) {
    return call->r_environment;
}

SEXP r_instrumentr_call_get_environment(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_environment(call);
}

/********************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_frame_position(instrumentr_call_t call) {
    return call->frame_position;
}

SEXP r_instrumentr_call_get_frame_position(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int frame_position = instrumentr_call_get_frame_position(call);
    return instrumentr_c_int_to_r_integer(frame_position);
}

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_call_is_active(instrumentr_call_t call) {
    return call->active;
}

SEXP r_instrumentr_call_is_active(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int active = instrumentr_call_is_active(call);
    return instrumentr_c_int_to_r_logical(active);
}

/* mutator  */
void instrumentr_call_activate(instrumentr_call_t call) {
    call->active = 1;
}

/* mutator  */
void instrumentr_call_deactivate(instrumentr_call_t call) {
    call->active = 0;
}

/********************************************************************************
 * r_result
 *******************************************************************************/

/* accessor  */
int instrumentr_call_has_result(instrumentr_call_t call) {
    return call->r_result != NULL;
}

SEXP r_instrumentr_call_has_result(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int result = instrumentr_call_has_result(call);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_call_get_result(instrumentr_call_t call) {
    if (instrumentr_call_has_result(call)) {
        return call->r_result;
    } else {
        instrumentr_log_error("call does not have a result");
        /* NOTE: never executed*/
        return NULL;
    }
}

SEXP r_instrumentr_call_get_result(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_result(call);
}

/* mutator */
void instrumentr_call_set_result(instrumentr_call_t call, SEXP r_result) {
    call->r_result = r_result;
    instrumentr_sexp_acquire(r_result);
}

/********************************************************************************
 * parameters
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_parameter_count(instrumentr_call_t call) {
    return call->parameters.length;
}

SEXP r_instrumentr_call_get_parameter_count(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int count = instrumentr_call_get_parameter_count(call);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_position(instrumentr_call_t call,
                                           int position) {
    int count = instrumentr_call_get_parameter_count(call);
    if (position < count && position >= 0) {
        return call->parameters.data[position];
    } else {
        instrumentr_log_error(
            "attempt to access %d parameter of a call with %d parameters",
            position,
            count);
        /* NOTE: never executed*/
        return NULL;
    }
}

SEXP r_instrumentr_call_get_parameter_by_position(SEXP r_call,
                                                  SEXP r_position) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_parameter_t parameter =
        instrumentr_call_get_parameter_by_position(call, position);
    return instrumentr_parameter_wrap(parameter);
}

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_name(instrumentr_call_t call,
                                       const char* name) {
    int count = instrumentr_call_get_parameter_count(call);
    instrumentr_parameter_t* parameters = call->parameters.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        if (strcmp(instrumentr_parameter_get_name(parameter), name) == 0) {
            return parameter;
        }
    }

    instrumentr_log_error(
        "parameter with name '%s' does not exist for this call",
        name);
    /* NOTE: never executed*/
    return NULL;
}

SEXP r_instrumentr_call_get_parameter_by_name(SEXP r_call, SEXP r_name) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_parameter_t parameter =
        instrumentr_call_get_parameter_by_name(call, name);
    return instrumentr_parameter_wrap(parameter);
}

/* accessor  */
SEXP r_instrumentr_call_get_parameters(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_parameter_t* parameters = call->parameters.data;
    int count = instrumentr_call_get_parameter_count(call);

    SEXP r_parameters = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        const char* name = instrumentr_parameter_get_name(parameter);
        SET_VECTOR_ELT(r_parameters, i, instrumentr_parameter_wrap(parameter));
        SET_STRING_ELT(r_names, i, mkChar(name));
    }

    Rf_setAttrib(r_parameters, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_parameters;
}

/* mutator  */
void instrumentr_call_append_parameter(instrumentr_call_t call,
                                       instrumentr_parameter_t parameter) {
    vec_push(&call->parameters, parameter);
    instrumentr_object_acquire(parameter);
}
