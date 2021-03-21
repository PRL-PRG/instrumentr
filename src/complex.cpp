#include "interop.h"
#include "utilities.h"
#include "complex.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_complex_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_complex_finalize(instrumentr_model_t model) {
    instrumentr_complex_t complex = (instrumentr_complex_t)(model);

    complex->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_complex_t instrumentr_complex_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr complex */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_complex_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_COMPLEX,
                                 instrumentr_complex_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_complex_t complex = (instrumentr_complex_t)(model);

    complex->r_sexp = r_sexp;

    return complex;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(complex, INSTRUMENTR_MODEL_TYPE_COMPLEX)

SEXP instrumentr_complex_get_sexp(instrumentr_complex_t complex) {
    return complex->r_sexp;
}

SEXP r_instrumentr_complex_get_sexp(SEXP r_complex) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    return instrumentr_complex_get_sexp(complex);
}

int instrumentr_complex_get_size(instrumentr_complex_t complex) {
    return Rf_length(complex->r_sexp);
}

SEXP r_instrumentr_complex_get_size(SEXP r_complex) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int result = instrumentr_complex_get_size(complex);
    return instrumentr_c_int_to_r_integer(result);
}

Rcomplex instrumentr_complex_get_element(instrumentr_complex_t complex,
                                         int index) {
    return COMPLEX_ELT(complex->r_sexp, index);
}

SEXP r_instrumentr_complex_get_element(SEXP r_complex, SEXP r_index) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int index = instrumentr_r_integer_to_c_int(r_index);
    Rcomplex result = instrumentr_complex_get_element(complex, index);
    return instrumentr_c_complex_to_r_complex(result);
}

bool instrumentr_complex_is_na(instrumentr_complex_t complex, int index) {
    Rcomplex result = COMPLEX_ELT(complex->r_sexp, index);
    return result.r == NA_REAL && result.i == NA_REAL;
}

SEXP r_instrumentr_complex_is_na(SEXP r_complex, SEXP r_index) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_complex_is_na(complex, index);
    return instrumentr_c_int_to_r_logical(result);
}
