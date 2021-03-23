#include "interop.h"
#include "utilities.h"
#include "weakref.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_weakref_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_weakref_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_weakref_t instrumentr_weakref_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    /* TODO: make foreign for instrumentr weakref */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_weakref_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_WEAKREF,
                                 instrumentr_weakref_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_weakref_t weakref = (instrumentr_weakref_t)(value);

    return weakref;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_WEAKREF, weakref, weakref)
