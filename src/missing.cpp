#include "interop.h"
#include "utilities.h"
#include "missing.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_missing_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_missing_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_missing_t instrumentr_missing_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr missing */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_missing_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_MISSING,
                                 instrumentr_missing_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_missing_t missing = (instrumentr_missing_t)(value);

    return missing;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_MISSING, missing, missing)
