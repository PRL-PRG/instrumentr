#include "interop.h"
#include "utilities.h"
#include "char.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_char_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_char_finalize(instrumentr_value_t value) {
    instrumentr_char_t charval = (instrumentr_char_t)(value);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_char_t instrumentr_char_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr char */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_char_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_CHAR,
                                 instrumentr_char_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_char_t charval = (instrumentr_char_t)(value);

    return charval;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_CHAR, char, charval)

const char* instrumentr_char_get_value(instrumentr_char_t charval) {
    return CHAR(instrumentr_char_get_sexp(charval));
}

SEXP r_instrumentr_char_get_value(SEXP r_char) {
    instrumentr_char_t charval = instrumentr_char_unwrap(r_char);
    const char* result = instrumentr_char_get_value(charval);
    return instrumentr_c_string_to_r_character(result);
}

bool instrumentr_char_is_na(instrumentr_char_t charval) {
    return instrumentr_char_get_sexp(charval) == NA_STRING;
}

SEXP r_instrumentr_char_is_na(SEXP r_char) {
    instrumentr_char_t charval = instrumentr_char_unwrap(r_char);
    bool result = instrumentr_char_is_na(charval);
    return instrumentr_c_int_to_r_logical(result);
}
