#include "interop.h"
#include "utilities.h"
#include "raw.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_raw_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_raw_finalize(instrumentr_model_t model) {
    instrumentr_raw_t raw = (instrumentr_raw_t)(model);

    raw->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_raw_t instrumentr_raw_create(instrumentr_state_t state,
                                         SEXP r_sexp) {
    /* TODO: make foreign for instrumentr raw */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_raw_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_RAW,
                                 instrumentr_raw_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_raw_t raw = (instrumentr_raw_t)(model);

    raw->r_sexp = r_sexp;

    return raw;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(raw, INSTRUMENTR_MODEL_TYPE_RAW)

SEXP instrumentr_raw_get_sexp(instrumentr_raw_t raw) {
    return raw->r_sexp;
}

SEXP r_instrumentr_raw_get_sexp(SEXP r_raw) {
    instrumentr_raw_t raw = instrumentr_raw_unwrap(r_raw);
    return instrumentr_raw_get_sexp(raw);
}

int instrumentr_raw_get_size(instrumentr_raw_t raw) {
    return Rf_length(raw->r_sexp);
}

SEXP r_instrumentr_raw_get_size(SEXP r_raw) {
    instrumentr_raw_t raw = instrumentr_raw_unwrap(r_raw);
    int result = instrumentr_raw_get_size(raw);
    return instrumentr_c_int_to_r_integer(result);
}

Rbyte instrumentr_raw_get_element(instrumentr_raw_t raw, int index) {
    return RAW_ELT(raw->r_sexp, index);
}

SEXP r_instrumentr_raw_get_element(SEXP r_raw, SEXP r_index) {
    instrumentr_raw_t raw = instrumentr_raw_unwrap(r_raw);
    int index = instrumentr_r_integer_to_c_int(r_index);
    Rbyte result = instrumentr_raw_get_element(raw, index);
    return instrumentr_c_raw_to_r_raw(result);
}
