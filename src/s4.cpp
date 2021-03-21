#include "interop.h"
#include "utilities.h"
#include "s4.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_s4_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_s4_finalize(instrumentr_model_t model) {
    instrumentr_s4_t s4 = (instrumentr_s4_t)(model);

    s4->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_s4_t instrumentr_s4_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr s4 */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_s4_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_S4,
                                 instrumentr_s4_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_s4_t s4 = (instrumentr_s4_t)(model);

    s4->r_sexp = r_sexp;

    return s4;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(s4, INSTRUMENTR_MODEL_TYPE_S4)

SEXP instrumentr_s4_get_sexp(instrumentr_s4_t s4) {
    return s4->r_sexp;
}

SEXP r_instrumentr_s4_get_sexp(SEXP r_s4) {
    instrumentr_s4_t s4 = instrumentr_s4_unwrap(r_s4);
    return instrumentr_s4_get_sexp(s4);
}
