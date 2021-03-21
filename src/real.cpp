#include "interop.h"
#include "utilities.h"
#include "real.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_real_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_real_finalize(instrumentr_model_t model) {
    instrumentr_real_t real = (instrumentr_real_t)(model);

    real->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_real_t instrumentr_real_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr real */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_real_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_REAL,
                                 instrumentr_real_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_real_t real = (instrumentr_real_t)(model);

    real->r_sexp = r_sexp;

    return real;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(real, INSTRUMENTR_MODEL_TYPE_REAL)

SEXP instrumentr_real_get_sexp(instrumentr_real_t real) {
    return real->r_sexp;
}

SEXP r_instrumentr_real_get_sexp(SEXP r_real) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    return instrumentr_real_get_sexp(real);
}

int instrumentr_real_get_size(instrumentr_real_t real) {
    return Rf_length(real->r_sexp);
}

SEXP r_instrumentr_real_get_size(SEXP r_real) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int result = instrumentr_real_get_size(real);
    return instrumentr_c_int_to_r_integer(result);
}

double instrumentr_real_get_element(instrumentr_real_t real, int index) {
    return REAL_ELT(real->r_sexp, index);
}

SEXP r_instrumentr_real_get_element(SEXP r_real, SEXP r_index) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int index = instrumentr_r_integer_to_c_int(r_index);
    double result = instrumentr_real_get_element(real, index);
    return instrumentr_c_double_to_r_double(result);
}

bool instrumentr_real_is_na(instrumentr_real_t real, int index) {
    return REAL_ELT(real->r_sexp, index) == NA_REAL;
}

SEXP r_instrumentr_real_is_na(SEXP r_real, SEXP r_index) {
    instrumentr_real_t real = instrumentr_real_unwrap(r_real);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_real_is_na(real, index);
    return instrumentr_c_int_to_r_logical(result);
}
