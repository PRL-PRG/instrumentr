#include "interop.h"
#include "utilities.h"
#include "s4.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_s4_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_s4_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_s4_t instrumentr_s4_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr s4 */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_s4_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_S4,
                                 instrumentr_s4_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_s4_t s4 = (instrumentr_s4_t) (value);

    return s4;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_S4, s4, s4)

instrumentr_value_t instrumentr_s4_get_data_slot(instrumentr_s4_t s4,
                                                 SEXPTYPE type) {
    SEXP r_result = dyntrace_get_s4_data_slot(instrumentr_s4_get_sexp(s4), type);
    instrumentr_state_t state = instrumentr_s4_get_state(s4);
    return instrumentr_state_value_table_lookup(state, r_result, 1);
}
