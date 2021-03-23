#include "utilities.h"
#include "pairlist.h"
#include "state.h"
#include "value.h"
#include "interop.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_pairlist_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_pairlist_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_pairlist_t instrumentr_pairlist_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr pairlist */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_pairlist_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_PAIRLIST,
                                 instrumentr_pairlist_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_pairlist_t pairlist = (instrumentr_pairlist_t)(value);

    return pairlist;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_PAIRLIST,
                             pairlist,
                             pairlist)

int instrumentr_pairlist_get_length(instrumentr_pairlist_t pairlist) {
    return Rf_length(instrumentr_pairlist_get_sexp(pairlist));
}

instrumentr_value_t
instrumentr_pairlist_get_car(instrumentr_pairlist_t pairlist) {
    SEXP result = CAR(instrumentr_pairlist_get_sexp(pairlist));
    instrumentr_state_t state = instrumentr_pairlist_get_state(pairlist);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_cdr(instrumentr_pairlist_t pairlist) {
    SEXP result = CDR(instrumentr_pairlist_get_sexp(pairlist));
    instrumentr_state_t state = instrumentr_pairlist_get_state(pairlist);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_tag(instrumentr_pairlist_t pairlist) {
    SEXP result = TAG(instrumentr_pairlist_get_sexp(pairlist));
    instrumentr_state_t state = instrumentr_pairlist_get_state(pairlist);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_element(instrumentr_pairlist_t pairlist, int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_pairlist_get_state(pairlist);

    for (i = 0, result = instrumentr_pairlist_get_sexp(pairlist);
         result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_pairlist_get_length(pairlist));

    return instrumentr_state_value_table_lookup(state, R_NilValue, 1);
}
