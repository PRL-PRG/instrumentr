#include "interop.h"
#include "utilities.h"
#include "expression.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_expression_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_expression_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_expression_t
instrumentr_expression_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr expression */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_expression_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_EXPRESSION,
                                 instrumentr_expression_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_expression_t expression = (instrumentr_expression_t)(value);

    return expression;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_EXPRESSION,
                             expression,
                             expression)

int instrumentr_expression_get_length(instrumentr_expression_t expression) {
    return Rf_length(instrumentr_expression_get_sexp(expression));
}

instrumentr_value_t
instrumentr_expression_get_element(instrumentr_expression_t expression,
                                   int index) {
    SEXP r_element =
        VECTOR_ELT(instrumentr_expression_get_sexp(expression), index);
    instrumentr_state_t state = instrumentr_expression_get_state(expression);
    return instrumentr_state_value_table_lookup(state, r_element, 1);
}
