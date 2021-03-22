#include "interop.h"
#include "utilities.h"
#include "dot.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_dot_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_dot_finalize(instrumentr_model_t model) {
    instrumentr_dot_t dot = (instrumentr_dot_t)(model);

    dot->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_dot_t instrumentr_dot_create(instrumentr_state_t state,
                                         SEXP r_sexp) {
    /* TODO: make foreign for instrumentr dot */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_dot_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_DOT,
                                 instrumentr_dot_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_dot_t dot = (instrumentr_dot_t)(model);

    dot->r_sexp = r_sexp;

    return dot;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(dot, INSTRUMENTR_MODEL_TYPE_DOT)

SEXP instrumentr_dot_get_sexp(instrumentr_dot_t dot) {
    return dot->r_sexp;
}

int instrumentr_dot_get_length(instrumentr_dot_t dot) {
    return Rf_length(dot->r_sexp);
}

instrumentr_value_t instrumentr_dot_get_car(instrumentr_dot_t dot) {
    SEXP result = CAR(dot->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_cdr(instrumentr_dot_t dot) {
    SEXP result = CDR(dot->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_tag(instrumentr_dot_t dot) {
    SEXP result = TAG(dot->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(result);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_element(instrumentr_dot_t dot,
                                                int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_model_get_state(dot);

    for (i = 0, result = dot->r_sexp; result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_dot_get_length(dot));

    return instrumentr_value_create(state, R_NilValue);
}
