#include "interop.h"
#include "utilities.h"
#include "externalptr.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_externalptr_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_externalptr_finalize(instrumentr_model_t model) {
    instrumentr_externalptr_t externalptr = (instrumentr_externalptr_t)(model);

    externalptr->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_externalptr_t
instrumentr_externalptr_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr externalptr */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_externalptr_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_EXTERNALPTR,
                                 instrumentr_externalptr_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_externalptr_t externalptr = (instrumentr_externalptr_t)(model);

    externalptr->r_sexp = r_sexp;

    return externalptr;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(externalptr,
                                     INSTRUMENTR_MODEL_TYPE_EXTERNALPTR)

SEXP instrumentr_externalptr_get_sexp(instrumentr_externalptr_t externalptr) {
    return externalptr->r_sexp;
}

SEXP r_instrumentr_externalptr_get_sexp(SEXP r_externalptr) {
    instrumentr_externalptr_t externalptr =
        instrumentr_externalptr_unwrap(r_externalptr);
    return instrumentr_externalptr_get_sexp(externalptr);
}
