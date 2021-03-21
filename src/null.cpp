#include "interop.h"
#include "utilities.h"
#include "null.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_null_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_null_finalize(instrumentr_model_t model) {
    instrumentr_null_t nullval = (instrumentr_null_t)(model);

    nullval->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_null_t instrumentr_null_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr null */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_null_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_NULL,
                                 instrumentr_null_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_null_t nullval = (instrumentr_null_t)(model);

    nullval->r_sexp = r_sexp;

    return nullval;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(null, INSTRUMENTR_MODEL_TYPE_NULL)

SEXP instrumentr_null_get_sexp(instrumentr_null_t nullval) {
    return nullval->r_sexp;
}

SEXP r_instrumentr_null_get_sexp(SEXP r_null) {
    instrumentr_null_t nullval = instrumentr_null_unwrap(r_null);
    return instrumentr_null_get_sexp(nullval);
}
