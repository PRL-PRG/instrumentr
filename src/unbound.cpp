#include "interop.h"
#include "utilities.h"
#include "unbound.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_unbound_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_unbound_finalize(instrumentr_model_t model) {
    instrumentr_unbound_t unbound = (instrumentr_unbound_t)(model);

    unbound->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_unbound_t instrumentr_unbound_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr unbound */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_unbound_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_UNBOUND,
                                 instrumentr_unbound_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_unbound_t unbound = (instrumentr_unbound_t)(model);

    unbound->r_sexp = r_sexp;

    return unbound;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(unbound, INSTRUMENTR_MODEL_TYPE_UNBOUND)

SEXP instrumentr_unbound_get_sexp(instrumentr_unbound_t unbound) {
    return unbound->r_sexp;
}

SEXP r_instrumentr_unbound_get_sexp(SEXP r_unbound) {
    instrumentr_unbound_t unbound = instrumentr_unbound_unwrap(r_unbound);
    return instrumentr_unbound_get_sexp(unbound);
}
