#include "value.h"
#include "interop.h"
#include "utilities.h"
#include "unbound.h"
#include "missing.h"
#include "null.h"
#include "externalptr.h"
#include "weakref.h"
#include "bytecode.h"
#include "s4.h"
#include "char.h"
#include "symbol.h"
#include "character.h"
#include "integer.h"
#include "real.h"
#include "logical.h"
#include "complex.h"
#include "raw.h"
#include "function.h"
#include "environment.h"
#include "miscellaneous.h"
#include "promise.h"
#include "language.h"
#include "dot.h"
#include "pairlist.h"
#include "list.h"
#include "expression.h"

/********************************************************************************
 * definition
 *******************************************************************************/

// struct instrumentr_value_t {
//    instrumentr_model_t model;
//};

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t instrumentr_value_create(instrumentr_state_t state,
                                             SEXP r_object) {
    instrumentr_model_t model = NULL;

    if (r_object == R_UnboundValue) {
        model =
            (instrumentr_model_t) instrumentr_unbound_create(state, r_object);
    }

    else if (r_object == R_MissingArg) {
        model =
            (instrumentr_model_t) instrumentr_missing_create(state, r_object);
    }

    else if (TYPEOF(r_object) == NILSXP) {
        model = (instrumentr_model_t) instrumentr_null_create(state, r_object);
    }

    else if (TYPEOF(r_object) == EXTPTRSXP) {
        model = (instrumentr_model_t) instrumentr_externalptr_create(state,
                                                                     r_object);
    }

    else if (TYPEOF(r_object) == WEAKREFSXP) {
        model =
            (instrumentr_model_t) instrumentr_weakref_create(state, r_object);
    }

    else if (TYPEOF(r_object) == BCODESXP) {
        model =
            (instrumentr_model_t) instrumentr_bytecode_create(state, r_object);
    }

    else if (TYPEOF(r_object) == S4SXP) {
        model = (instrumentr_model_t) instrumentr_s4_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CHARSXP) {
        model = (instrumentr_model_t) instrumentr_char_create(state, r_object);
    }

    else if (TYPEOF(r_object) == SYMSXP) {
        model =
            (instrumentr_model_t) instrumentr_symbol_create(state, r_object);
    }

    else if (TYPEOF(r_object) == STRSXP) {
        model =
            (instrumentr_model_t) instrumentr_character_create(state, r_object);
    }

    else if (TYPEOF(r_object) == PROMSXP) {
        model =
            (instrumentr_model_t) instrumentr_promise_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CLOSXP) {
        model =
            (instrumentr_model_t) instrumentr_function_create(state, r_object);
    }

    else if (TYPEOF(r_object) == SPECIALSXP) {
        model =
            (instrumentr_model_t) instrumentr_function_create(state, r_object);
    }

    else if (TYPEOF(r_object) == BUILTINSXP) {
        model =
            (instrumentr_model_t) instrumentr_function_create(state, r_object);
    }

    else if (TYPEOF(r_object) == ENVSXP) {
        model = (instrumentr_model_t) instrumentr_environment_create(state,
                                                                     r_object);
    }

    else if (TYPEOF(r_object) == INTSXP) {
        model =
            (instrumentr_model_t) instrumentr_integer_create(state, r_object);
    }

    else if (TYPEOF(r_object) == REALSXP) {
        model = (instrumentr_model_t) instrumentr_real_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LGLSXP) {
        model =
            (instrumentr_model_t) instrumentr_logical_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CPLXSXP) {
        model =
            (instrumentr_model_t) instrumentr_complex_create(state, r_object);
    }

    else if (TYPEOF(r_object) == RAWSXP) {
        model = (instrumentr_model_t) instrumentr_raw_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LANGSXP) {
        model =
            (instrumentr_model_t) instrumentr_language_create(state, r_object);
    }

    else if (TYPEOF(r_object) == DOTSXP) {
        model = (instrumentr_model_t) instrumentr_dot_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LISTSXP) {
        model =
            (instrumentr_model_t) instrumentr_pairlist_create(state, r_object);
    }

    else if (TYPEOF(r_object) == VECSXP) {
        model = (instrumentr_model_t) instrumentr_list_create(state, r_object);
    }

    else if (TYPEOF(r_object) == EXPRSXP) {
        model = (instrumentr_model_t) instrumentr_expression_create(state,
                                                                    r_object);
    }

    else {
        model = (instrumentr_model_t) instrumentr_miscellaneous_create(
            state, r_object);
    }

    instrumentr_value_t value;
    value.model = model;
    return value;
}

/********************************************************************************
 * type
 *******************************************************************************/

instrumentr_model_type_t instrumentr_value_get_type(instrumentr_value_t value) {
    return instrumentr_model_get_type(value.model);
}

#define INSTRUMENTR_VALUE_DEFINE_API(MODEL_TYPE, MODEL_NAME)            \
    bool instrumentr_value_is_##MODEL_NAME(instrumentr_value_t value) { \
        return instrumentr_model_get_type(value.model) == MODEL_TYPE;   \
    }                                                                   \
                                                                        \
    instrumentr_##MODEL_NAME##_t instrumentr_value_as_##MODEL_NAME(     \
        instrumentr_value_t value) {                                    \
        return (instrumentr_##MODEL_NAME##_t)(value.model);             \
    }

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_APPLICATION, application)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_UNBOUND, unbound)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_MISSING, missing)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_NULL, null)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_EXTERNALPTR, externalptr)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_WEAKREF, weakref)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_BYTECODE, bytecode)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_S4, s4)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_CHAR, char)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_SYMBOL, symbol)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_CHARACTER, character)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_INTEGER, integer)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_REAL, real)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_LOGICAL, logical)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_COMPLEX, complex)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_RAW, raw)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_ENVIRONMENT, environment)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_FUNCTION, function)
INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_MODEL_TYPE_PROMISE, promise)
