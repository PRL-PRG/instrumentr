#include "interop.h"
#include "utilities.h"
#include "pairlist.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_pairlist_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_pairlist_finalize(instrumentr_model_t model) {
    instrumentr_pairlist_t pairlist = (instrumentr_pairlist_t)(model);

    pairlist->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_pairlist_t instrumentr_pairlist_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr pairlist */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_pairlist_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_PAIRLIST,
                                 instrumentr_pairlist_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_pairlist_t pairlist = (instrumentr_pairlist_t)(model);

    pairlist->r_sexp = r_sexp;

    return pairlist;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(pairlist, INSTRUMENTR_MODEL_TYPE_PAIRLIST)

SEXP instrumentr_pairlist_get_sexp(instrumentr_pairlist_t pairlist) {
    return pairlist->r_sexp;
}

int instrumentr_pairlist_get_length(instrumentr_pairlist_t pairlist) {
    return Rf_length(pairlist->r_sexp);
}

instrumentr_value_t
instrumentr_pairlist_get_car(instrumentr_pairlist_t pairlist) {
    SEXP result = CAR(pairlist->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_cdr(instrumentr_pairlist_t pairlist) {
    SEXP result = CDR(pairlist->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_tag(instrumentr_pairlist_t pairlist) {
    SEXP result = TAG(pairlist->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t
instrumentr_pairlist_get_element(instrumentr_pairlist_t pairlist, int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_model_get_state(pairlist);

    for (i = 0, result = pairlist->r_sexp; result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_pairlist_get_length(pairlist));

    return instrumentr_value_create(state, R_NilValue);
}
