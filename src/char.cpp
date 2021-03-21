#include "interop.h"
#include "utilities.h"
#include "char.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_char_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_char_finalize(instrumentr_model_t model) {
    instrumentr_char_t charval = (instrumentr_char_t)(model);

    charval->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_char_t instrumentr_char_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr char */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_char_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CHAR,
                                 instrumentr_char_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_char_t charval = (instrumentr_char_t)(model);

    charval->r_sexp = r_sexp;

    return charval;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(char, INSTRUMENTR_MODEL_TYPE_CHAR)

SEXP instrumentr_char_get_sexp(instrumentr_char_t charval) {
    return charval->r_sexp;
}

SEXP r_instrumentr_char_get_sexp(SEXP r_char) {
    instrumentr_char_t charval = instrumentr_char_unwrap(r_char);
    return instrumentr_char_get_sexp(charval);
}

const char* instrumentr_char_get_value(instrumentr_char_t charval) {
    return CHAR(charval->r_sexp);
}

SEXP r_instrumentr_char_get_value(SEXP r_char) {
    instrumentr_char_t charval = instrumentr_char_unwrap(r_char);
    const char* result = instrumentr_char_get_value(charval);
    return instrumentr_c_string_to_r_character(result);
}

bool instrumentr_char_is_na(instrumentr_char_t charval) {
    return charval->r_sexp == NA_STRING;
}

SEXP r_instrumentr_char_is_na(SEXP r_char) {
    instrumentr_char_t charval = instrumentr_char_unwrap(r_char);
    bool result = instrumentr_char_is_na(charval);
    return instrumentr_c_int_to_r_logical(result);
}
