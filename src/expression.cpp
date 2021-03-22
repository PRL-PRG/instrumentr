#include "interop.h"
#include "utilities.h"
#include "expression.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_expression_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_expression_finalize(instrumentr_model_t model) {
    instrumentr_expression_t expression = (instrumentr_expression_t)(model);

    expression->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_expression_t
instrumentr_expression_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr expression */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_expression_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_EXPRESSION,
                                 instrumentr_expression_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_expression_t expression = (instrumentr_expression_t)(model);

    expression->r_sexp = r_sexp;

    return expression;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(expression,
                                     INSTRUMENTR_MODEL_TYPE_EXPRESSION)

SEXP instrumentr_expression_get_sexp(instrumentr_expression_t expression) {
    return expression->r_sexp;
}

int instrumentr_expression_get_length(instrumentr_expression_t expression) {
    return Rf_length(expression->r_sexp);
}

instrumentr_value_t
instrumentr_expression_get_element(instrumentr_expression_t expression,
                                   int index) {
    SEXP r_element = VECTOR_ELT(expression->r_sexp, index);
    instrumentr_state_t state = instrumentr_model_get_state(expression);
    return instrumentr_state_value_table_lookup(state, r_element, 1);
}
