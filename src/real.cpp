#include "interop.h"
#include "utilities.h"
#include "real.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_real_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_real_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_real_t instrumentr_real_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr real */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_real_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_REAL,
                                 instrumentr_real_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_real_t real = (instrumentr_real_t)(value);

    return real;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_REAL, real, real)

int instrumentr_real_get_size(instrumentr_real_t real) {
    return Rf_length(instrumentr_real_get_sexp(real));
}

SEXP r_instrumentr_real_get_size(SEXP r_real) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int result = instrumentr_real_get_size(real);
    return instrumentr_c_int_to_r_integer(result);
}

double instrumentr_real_get_element(instrumentr_real_t real, int index) {
    return REAL_ELT(instrumentr_real_get_sexp(real), index);
}

SEXP r_instrumentr_real_get_element(SEXP r_real, SEXP r_index) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int index = instrumentr_r_integer_to_c_int(r_index);
    double result = instrumentr_real_get_element(real, index);
    return instrumentr_c_double_to_r_double(result);
}

bool instrumentr_real_is_na(instrumentr_real_t real, int index) {
    return REAL_ELT(instrumentr_real_get_sexp(real), index) == NA_REAL;
}

SEXP r_instrumentr_real_is_na(SEXP r_real, SEXP r_index) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_real_is_na(real, index);
    return instrumentr_c_int_to_r_logical(result);
}
