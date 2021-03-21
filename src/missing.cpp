#include "interop.h"
#include "utilities.h"
#include "missing.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_missing_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_missing_finalize(instrumentr_model_t model) {
    instrumentr_missing_t missing = (instrumentr_missing_t)(model);

    missing->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_missing_t instrumentr_missing_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr missing */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_missing_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_MISSING,
                                 instrumentr_missing_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_missing_t missing = (instrumentr_missing_t)(model);

    missing->r_sexp = r_sexp;

    return missing;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(missing, INSTRUMENTR_MODEL_TYPE_MISSING)

SEXP instrumentr_missing_get_sexp(instrumentr_missing_t missing) {
    return missing->r_sexp;
}

SEXP r_instrumentr_missing_get_sexp(SEXP r_missing) {
    instrumentr_missing_t missing = instrumentr_missing_unwrap(r_missing);
    return instrumentr_missing_get_sexp(missing);
}
