#include "interop.h"
#include "utilities.h"
#include "integer.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_integer_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_integer_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_integer_t instrumentr_integer_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr integer */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_integer_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_INTEGER,
                                 instrumentr_integer_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_integer_t integer = (instrumentr_integer_t)(value);

    return integer;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_INTEGER, integer, integer)

int instrumentr_integer_get_size(instrumentr_integer_t integer) {
    return Rf_length(instrumentr_integer_get_sexp(integer));
}

SEXP r_instrumentr_integer_get_size(SEXP r_integer) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int result = instrumentr_integer_get_size(integer);
    return instrumentr_c_int_to_r_integer(result);
}

int instrumentr_integer_get_element(instrumentr_integer_t integer, int index) {
    return INTEGER(instrumentr_integer_get_sexp(integer))[index];
}

SEXP r_instrumentr_integer_get_element(SEXP r_integer, SEXP r_index) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int index = instrumentr_r_integer_to_c_int(r_index);
    int result = instrumentr_integer_get_element(integer, index);
    return instrumentr_c_int_to_r_integer(result);
}

bool instrumentr_integer_is_na(instrumentr_integer_t integer, int index) {
    return INTEGER(instrumentr_integer_get_sexp(integer))[index] == NA_INTEGER;
}

SEXP r_instrumentr_integer_is_na(SEXP r_integer, SEXP r_index) {
    instrumentr_integer_t integer = instrumentr_integer_unwrap(r_integer);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_integer_is_na(integer, index);
    return instrumentr_c_int_to_r_logical(result);
}
