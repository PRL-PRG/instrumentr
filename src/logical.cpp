#include "interop.h"
#include "utilities.h"
#include "logical.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_logical_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_logical_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_logical_t instrumentr_logical_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr logical */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_logical_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_LOGICAL,
                                 instrumentr_logical_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_logical_t logical = (instrumentr_logical_t)(value);

    return logical;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_LOGICAL, logical, logical)

int instrumentr_logical_get_size(instrumentr_logical_t logical) {
    return Rf_length(instrumentr_logical_get_sexp(logical));
}

SEXP r_instrumentr_logical_get_size(SEXP r_logical) {
    instrumentr_logical_t logical = instrumentr_logical_unwrap(r_logical);
    int result = instrumentr_logical_get_size(logical);
    return instrumentr_c_int_to_r_integer(result);
}

int instrumentr_logical_get_element(instrumentr_logical_t logical, int index) {
    return LOGICAL_ELT(instrumentr_logical_get_sexp(logical), index);
}

SEXP r_instrumentr_logical_get_element(SEXP r_logical, SEXP r_index) {
    instrumentr_logical_t logical = instrumentr_logical_unwrap(r_logical);
    int index = instrumentr_r_integer_to_c_int(r_index);
    int result = instrumentr_logical_get_element(logical, index);
    return instrumentr_c_int_to_r_logical(result);
}

bool instrumentr_logical_is_na(instrumentr_logical_t logical, int index) {
    return LOGICAL_ELT(instrumentr_logical_get_sexp(logical), index) ==
           NA_LOGICAL;
}

SEXP r_instrumentr_logical_is_na(SEXP r_logical, SEXP r_index) {
    instrumentr_logical_t logical = instrumentr_logical_unwrap(r_logical);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_logical_is_na(logical, index);
    return instrumentr_c_int_to_r_logical(result);
}
