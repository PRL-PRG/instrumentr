#include "interop.h"
#include "utilities.h"
#include "language.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_language_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_language_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_language_t instrumentr_language_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr language */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_language_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_LANGUAGE,
                                 instrumentr_language_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_language_t language = (instrumentr_language_t)(value);

    return language;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_LANGUAGE,
                             language,
                             language)

int instrumentr_language_get_length(instrumentr_language_t language) {
    return Rf_length(instrumentr_language_get_sexp(language));
}

instrumentr_value_t
instrumentr_language_get_car(instrumentr_language_t language) {
    SEXP result = CAR(instrumentr_language_get_sexp(language));
    instrumentr_state_t state = instrumentr_language_get_state(language);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_cdr(instrumentr_language_t language) {
    SEXP result = CDR(instrumentr_language_get_sexp(language));
    instrumentr_state_t state = instrumentr_language_get_state(language);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_tag(instrumentr_language_t language) {
    SEXP result = TAG(instrumentr_language_get_sexp(language));
    instrumentr_state_t state = instrumentr_language_get_state(language);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_language_get_element(instrumentr_language_t language, int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_language_get_state(language);

    for (i = 0, result = instrumentr_language_get_sexp(language);
         result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_language_get_length(language));

    return instrumentr_state_value_table_lookup(state, R_NilValue, 1);
}
