#include "interop.h"
#include "utilities.h"
#include "raw.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_raw_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_raw_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_raw_t instrumentr_raw_create(instrumentr_state_t state,
                                         SEXP r_sexp) {
    /* TODO: make foreign for instrumentr raw */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_raw_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_RAW,
                                 instrumentr_raw_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_raw_t raw = (instrumentr_raw_t)(value);

    return raw;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_RAW, raw, raw)

int instrumentr_raw_get_size(instrumentr_raw_t raw) {
    return Rf_length(instrumentr_raw_get_sexp(raw));
}

SEXP r_instrumentr_raw_get_size(SEXP r_raw) {
    instrumentr_raw_t raw = instrumentr_raw_unwrap(r_raw);
    int result = instrumentr_raw_get_size(raw);
    return instrumentr_c_int_to_r_integer(result);
}

Rbyte instrumentr_raw_get_element(instrumentr_raw_t raw, int index) {
    return RAW_ELT(instrumentr_raw_get_sexp(raw), index);
}

SEXP r_instrumentr_raw_get_element(SEXP r_raw, SEXP r_index) {
    instrumentr_raw_t raw = instrumentr_raw_unwrap(r_raw);
    int index = instrumentr_r_integer_to_c_int(r_index);
    Rbyte result = instrumentr_raw_get_element(raw, index);
    return instrumentr_c_raw_to_r_raw(result);
}
