#include "value.h"
#include "interop.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_value_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_value_finalize(instrumentr_model_t model) {
    instrumentr_value_t value = (instrumentr_value_t)(model);

    value->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t instrumentr_value_create(instrumentr_state_t state,
                                             SEXP r_sexp) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_value_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_VALUE,
                                 instrumentr_value_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_value_t value = (instrumentr_value_t)(model);

    value->r_sexp = r_sexp;

    return value;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(value, INSTRUMENTR_MODEL_TYPE_VALUE)

/********************************************************************************
 * r_sexp
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_value_get_sexp(instrumentr_value_t value) {
    return value->r_sexp;
}

SEXP r_instrumentr_value_get_sexp(SEXP r_value) {
    instrumentr_value_t value = instrumentr_value_unwrap(r_value);
    SEXP r_sexp = instrumentr_value_get_sexp(value);
    SEXPTYPE type = instrumentr_value_get_sexp_type(value);

    /* NOTE: quote language and symbol to prevent them from evaluation */
    if (type == LANGSXP || type == SYMSXP) {
        r_sexp = instrumentr_sexp_quote(r_sexp);
    }

    return r_sexp;
}

const char* instrumentr_value_get_sexp_address(instrumentr_value_t value) {
    SEXP r_sexp = instrumentr_value_get_sexp(value);
    return address_to_string(r_sexp);
}

SEXP r_instrumentr_value_get_sexp_address(SEXP r_value) {
    instrumentr_value_t value = instrumentr_value_unwrap(r_value);
    const char* address = instrumentr_value_get_sexp_address(value);
    return instrumentr_c_string_to_r_character(address);
}

SEXPTYPE instrumentr_value_get_sexp_type(instrumentr_value_t value) {
    return TYPEOF(value->r_sexp);
}

SEXP r_instrumentr_value_get_sexp_type(SEXP r_value) {
    instrumentr_value_t value = instrumentr_value_unwrap(r_value);
    SEXPTYPE type = instrumentr_value_get_sexp_type(value);
    const char* type_str = Rf_type2char(type);
    return instrumentr_c_string_to_r_character(type_str);
}