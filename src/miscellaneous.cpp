#include "miscellaneous.h"
#include "interop.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_miscellaneous_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_miscellaneous_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_miscellaneous_t
instrumentr_miscellaneous_create(instrumentr_state_t state, SEXP r_sexp) {
    instrumentr_value_t value = instrumentr_value_create(
        state,
        sizeof(struct instrumentr_miscellaneous_impl_t),
        INSTRUMENTR_VALUE_TYPE_MISCELLANEOUS,
        instrumentr_miscellaneous_finalize,
        INSTRUMENTR_ORIGIN_LOCAL,
        r_sexp);

    instrumentr_miscellaneous_t miscellaneous =
        (instrumentr_miscellaneous_t)(value);

    return miscellaneous;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_MISCELLANEOUS,
                             miscellaneous,
                             miscellaneous)
