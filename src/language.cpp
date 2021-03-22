#include "interop.h"
#include "utilities.h"
#include "language.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_language_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_language_finalize(instrumentr_model_t model) {
    instrumentr_language_t language = (instrumentr_language_t)(model);

    language->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_language_t instrumentr_language_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr language */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_language_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_LANGUAGE,
                                 instrumentr_language_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_language_t language = (instrumentr_language_t)(model);

    language->r_sexp = r_sexp;

    return language;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(language, INSTRUMENTR_MODEL_TYPE_LANGUAGE)

SEXP instrumentr_language_get_sexp(instrumentr_language_t language) {
    return language->r_sexp;
}

int instrumentr_language_get_length(instrumentr_language_t language) {
    return Rf_length(language->r_sexp);
}

instrumentr_value_t
instrumentr_language_get_car(instrumentr_language_t language) {
    SEXP result = CAR(language->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_cdr(instrumentr_language_t language) {
    SEXP result = CDR(language->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_tag(instrumentr_language_t language) {
    SEXP result = TAG(language->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_element(instrumentr_language_t language, int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_model_get_state(language);

    for (i = 0, result = language->r_sexp; result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_language_get_length(language));

    return instrumentr_value_create(state, R_NilValue);
}
