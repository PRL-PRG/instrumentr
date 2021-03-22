#include "interop.h"
#include "utilities.h"
#include "list.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_list_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_list_finalize(instrumentr_model_t model) {
    instrumentr_list_t list = (instrumentr_list_t)(model);

    list->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_list_t instrumentr_list_create(instrumentr_state_t state,
                                           SEXP r_sexp) {
    /* TODO: make foreign for instrumentr list */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_list_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_LIST,
                                 instrumentr_list_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_list_t list = (instrumentr_list_t)(model);

    list->r_sexp = r_sexp;

    return list;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(list, INSTRUMENTR_MODEL_TYPE_LIST)

SEXP instrumentr_list_get_sexp(instrumentr_list_t list) {
    return list->r_sexp;
}

int instrumentr_list_get_length(instrumentr_list_t list) {
    return Rf_length(list->r_sexp);
}

instrumentr_value_t instrumentr_list_get_element(instrumentr_list_t list,
                                                 int index) {
    SEXP r_element = VECTOR_ELT(list->r_sexp, index);
    instrumentr_state_t state = instrumentr_model_get_state(list);
    return instrumentr_state_value_table_lookup(state, r_element, 1);
}
