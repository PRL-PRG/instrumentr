#include "utilities.h"
#include "null.h"
#include "interop.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_null_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_null_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_null_t instrumentr_null_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr null */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_null_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_NULL,
                                 instrumentr_null_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_null_t nullval = (instrumentr_null_t)(value);

    return nullval;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_NULL, null, nullval)
