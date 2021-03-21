#include "interop.h"
#include "utilities.h"
#include "weakref.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_weakref_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_weakref_finalize(instrumentr_model_t model) {
    instrumentr_weakref_t weakref = (instrumentr_weakref_t)(model);

    weakref->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_weakref_t instrumentr_weakref_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr weakref */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_weakref_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_WEAKREF,
                                 instrumentr_weakref_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_weakref_t weakref = (instrumentr_weakref_t)(model);

    weakref->r_sexp = r_sexp;

    return weakref;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(weakref, INSTRUMENTR_MODEL_TYPE_WEAKREF)

SEXP instrumentr_weakref_get_sexp(instrumentr_weakref_t weakref) {
    return weakref->r_sexp;
}

SEXP r_instrumentr_weakref_get_sexp(SEXP r_weakref) {
    instrumentr_weakref_t weakref = instrumentr_weakref_unwrap(r_weakref);
    return instrumentr_weakref_get_sexp(weakref);
}
