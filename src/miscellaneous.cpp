#include "miscellaneous.h"
#include "interop.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_miscellaneous_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_miscellaneous;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_miscellaneous_finalize(instrumentr_model_t model) {
    instrumentr_miscellaneous_t miscellaneous =
        (instrumentr_miscellaneous_t)(model);

    miscellaneous->r_miscellaneous = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_miscellaneous_t
instrumentr_miscellaneous_create(instrumentr_state_t state,
                                 SEXP r_miscellaneous) {
    instrumentr_model_t model = instrumentr_model_create(
        state,
        sizeof(struct instrumentr_miscellaneous_impl_t),
        INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS,
        instrumentr_miscellaneous_finalize,
        INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_miscellaneous_t miscellaneous =
        (instrumentr_miscellaneous_t)(model);

    miscellaneous->r_miscellaneous = r_miscellaneous;

    return miscellaneous;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(miscellaneous,
                                     INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS)
