#include "interop.h"
#include "utilities.h"
#include "complex.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_complex_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_complex_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_complex_t instrumentr_complex_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr complex */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_complex_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_COMPLEX,
                                 instrumentr_complex_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_complex_t complex = (instrumentr_complex_t)(value);

    return complex;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_COMPLEX, complex, complex)

int instrumentr_complex_get_size(instrumentr_complex_t complex) {
    return Rf_length(instrumentr_complex_get_sexp(complex));
}

SEXP r_instrumentr_complex_get_size(SEXP r_complex) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int result = instrumentr_complex_get_size(complex);
    return instrumentr_c_int_to_r_integer(result);
}

Rcomplex instrumentr_complex_get_element(instrumentr_complex_t complex,
                                         int index) {
    return COMPLEX_ELT(instrumentr_complex_get_sexp(complex), index);
}

SEXP r_instrumentr_complex_get_element(SEXP r_complex, SEXP r_index) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int index = instrumentr_r_integer_to_c_int(r_index);
    Rcomplex result = instrumentr_complex_get_element(complex, index);
    return instrumentr_c_complex_to_r_complex(result);
}

bool instrumentr_complex_is_na(instrumentr_complex_t complex, int index) {
    Rcomplex result = COMPLEX_ELT(instrumentr_complex_get_sexp(complex), index);
    return result.r == NA_REAL && result.i == NA_REAL;
}

SEXP r_instrumentr_complex_is_na(SEXP r_complex, SEXP r_index) {
    instrumentr_complex_t complex = instrumentr_complex_unwrap(r_complex);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_complex_is_na(complex, index);
    return instrumentr_c_int_to_r_logical(result);
}
