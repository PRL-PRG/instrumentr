#include "interop.h"
#include "utilities.h"
#include "list.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_list_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_list_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_list_t instrumentr_list_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr list */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_list_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_LIST,
                                 instrumentr_list_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_list_t list = (instrumentr_list_t)(value);

    return list;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_LIST, list, list)

int instrumentr_list_get_length(instrumentr_list_t list) {
    return Rf_length(instrumentr_list_get_sexp(list));
}

instrumentr_value_t instrumentr_list_get_element(instrumentr_list_t list,
                                                 int index) {
    SEXP r_element = VECTOR_ELT(instrumentr_list_get_sexp(list), index);
    instrumentr_state_t state = instrumentr_list_get_state(list);
    return instrumentr_state_value_table_lookup(state, r_element, 1);
}
