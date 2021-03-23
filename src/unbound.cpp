#include "interop.h"
#include "utilities.h"
#include "unbound.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_unbound_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_unbound_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_unbound_t instrumentr_unbound_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr unbound */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_unbound_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_UNBOUND,
                                 instrumentr_unbound_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_unbound_t unbound = (instrumentr_unbound_t)(value);

    return unbound;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_UNBOUND, unbound, unbound)
