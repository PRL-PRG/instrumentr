#include "interop.h"
#include "utilities.h"
#include "integer.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_integer_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_integer_finalize(instrumentr_model_t model) {
    instrumentr_integer_t integer = (instrumentr_integer_t)(model);

    integer->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_integer_t instrumentr_integer_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr integer */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_integer_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_INTEGER,
                                 instrumentr_integer_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_integer_t integer = (instrumentr_integer_t)(model);

    integer->r_sexp = r_sexp;

    return integer;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(integer, INSTRUMENTR_MODEL_TYPE_INTEGER)

SEXP instrumentr_integer_get_sexp(instrumentr_integer_t integer) {
    return integer->r_sexp;
}

SEXP r_instrumentr_integer_get_sexp(SEXP r_integer) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    return instrumentr_integer_get_sexp(integer);
}

int instrumentr_integer_get_size(instrumentr_integer_t integer) {
    return Rf_length(integer->r_sexp);
}

SEXP r_instrumentr_integer_get_size(SEXP r_integer) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int result = instrumentr_integer_get_size(integer);
    return instrumentr_c_int_to_r_integer(result);
}

int instrumentr_integer_get_element(instrumentr_integer_t integer, int index) {
    return INTEGER(integer->r_sexp)[index];
}

SEXP r_instrumentr_integer_get_element(SEXP r_integer, SEXP r_index) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int index = instrumentr_r_integer_to_c_int(r_index);
    int result = instrumentr_integer_get_element(integer, index);
    return instrumentr_c_int_to_r_integer(result);
}

bool instrumentr_integer_is_na(instrumentr_integer_t integer, int index) {
    return INTEGER(integer->r_sexp)[index] == NA_INTEGER;
}

SEXP r_instrumentr_integer_is_na(SEXP r_integer, SEXP r_index) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_integer_is_na(integer, index);
    return instrumentr_c_int_to_r_logical(result);
}
