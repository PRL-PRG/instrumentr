#include "interop.h"
#include "utilities.h"
#include "externalptr.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_externalptr_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_externalptr_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_externalptr_t
instrumentr_externalptr_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr externalptr */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_externalptr_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_EXTERNALPTR,
                                 instrumentr_externalptr_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_externalptr_t externalptr = (instrumentr_externalptr_t)(value);

    return externalptr;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_EXTERNALPTR,
                             externalptr,
                             externalptr)
