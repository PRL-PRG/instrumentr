#include "interop.h"
#include "utilities.h"
#include "dot.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_dot_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_dot_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_dot_t instrumentr_dot_create(instrumentr_state_t state,
                                         SEXP r_sexp) {
    /* TODO: make foreign for instrumentr dot */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_dot_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_DOT,
                                 instrumentr_dot_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_dot_t dot = (instrumentr_dot_t)(value);

    return dot;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_DOT, dot, dot)

int instrumentr_dot_get_length(instrumentr_dot_t dot) {
    return Rf_length(instrumentr_dot_get_sexp(dot));
}

instrumentr_value_t instrumentr_dot_get_car(instrumentr_dot_t dot) {
    SEXP result = CAR(instrumentr_dot_get_sexp(dot));
    instrumentr_state_t state = instrumentr_dot_get_state(dot);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_cdr(instrumentr_dot_t dot) {
    SEXP result = CDR(instrumentr_dot_get_sexp(dot));
    instrumentr_state_t state = instrumentr_dot_get_state(dot);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_tag(instrumentr_dot_t dot) {
    SEXP result = TAG(instrumentr_dot_get_sexp(dot));
    instrumentr_state_t state = instrumentr_dot_get_state(dot);
    return instrumentr_state_value_table_lookup(state, result, 1);
}

instrumentr_value_t instrumentr_dot_get_element(instrumentr_dot_t dot,
                                                int index) {
    int i = 0;
    SEXP result = R_NilValue;
    instrumentr_state_t state = instrumentr_dot_get_state(dot);

    for (i = 0, result = instrumentr_dot_get_sexp(dot); result != R_NilValue;
         result = CDR(result), ++i) {
        if (i == index) {
            return instrumentr_state_value_table_lookup(state, result, 1);
        }
    }

    instrumentr_log_error(
        "cannot get element at position %d from a %d element list",
        index,
        instrumentr_dot_get_length(dot));

    return instrumentr_state_value_table_lookup(state, R_NilValue, 1);
}
