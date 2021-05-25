#include "eval.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "value.h"
#include "environment.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_eval_impl_t {
    struct instrumentr_model_impl_t model;
    instrumentr_value_t expression;
    instrumentr_environment_t environment;
    int active;
    instrumentr_value_t result;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_eval_finalize(instrumentr_model_t model) {
    instrumentr_eval_t eval = (instrumentr_eval_t) (model);

    instrumentr_value_release(eval->expression);
    eval->expression = NULL;

    instrumentr_environment_release(eval->environment);
    eval->environment = NULL;

    eval->active = 0;

    if (eval->result != NULL) {
        instrumentr_value_release(eval->result);
        eval->result = NULL;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_eval_t instrumentr_eval_create(instrumentr_state_t state,
                                           SEXP r_expression,
                                           SEXP r_environment) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_eval_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_EVAL,
                                 instrumentr_eval_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_eval_t eval = (instrumentr_eval_t) (model);

    eval->expression =
        instrumentr_state_value_table_lookup(state, r_expression, 1);
    instrumentr_value_acquire(eval->expression);

    eval->environment = instrumentr_state_value_table_lookup_environment(
        state, r_environment, 1);
    instrumentr_environment_acquire(eval->environment);

    eval->active = 0;

    eval->result = NULL;

    return eval;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_DEFINE_DERIVED_API(INSTRUMENTR_MODEL_TYPE_EVAL, eval, eval)

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
instrumentr_value_t instrumentr_eval_get_expression(instrumentr_eval_t eval) {
    return eval->expression;
}

SEXP r_instrumentr_eval_get_expression(SEXP r_eval) {
    instrumentr_eval_t eval = instrumentr_eval_unwrap(r_eval);
    instrumentr_value_t expression = instrumentr_eval_get_expression(eval);
    return instrumentr_value_wrap(expression);
}

/********************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
instrumentr_environment_t
instrumentr_eval_get_environment(instrumentr_eval_t eval) {
    return eval->environment;
}

SEXP r_instrumentr_eval_get_environment(SEXP r_eval) {
    instrumentr_eval_t eval = instrumentr_eval_unwrap(r_eval);
    instrumentr_environment_t environment =
        instrumentr_eval_get_environment(eval);
    return instrumentr_environment_wrap(environment);
}

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_eval_is_active(instrumentr_eval_t eval) {
    return eval->active;
}

SEXP r_instrumentr_eval_is_active(SEXP r_eval) {
    instrumentr_eval_t eval = instrumentr_eval_unwrap(r_eval);
    int active = instrumentr_eval_is_active(eval);
    return instrumentr_c_int_to_r_logical(active);
}

/* mutator  */
void instrumentr_eval_activate(instrumentr_eval_t eval) {
    eval->active = 1;
}

/* mutator  */
void instrumentr_eval_deactivate(instrumentr_eval_t eval) {
    eval->active = 0;
}

/********************************************************************************
 * result
 *******************************************************************************/

/* accessor  */
int instrumentr_eval_has_result(instrumentr_eval_t eval) {
    return eval->result != NULL;
}

SEXP r_instrumentr_eval_has_result(SEXP r_eval) {
    instrumentr_eval_t eval = instrumentr_eval_unwrap(r_eval);
    int result = instrumentr_eval_has_result(eval);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_value_t instrumentr_eval_get_result(instrumentr_eval_t eval) {
    if (instrumentr_eval_has_result(eval)) {
        return eval->result;
    } else {
        instrumentr_log_error("eval does not have a result");
        /* NOTE: never executed*/
        return NULL;
    }
}

SEXP r_instrumentr_eval_get_result(SEXP r_eval) {
    instrumentr_eval_t eval = instrumentr_eval_unwrap(r_eval);
    instrumentr_value_t result = instrumentr_eval_get_result(eval);
    return instrumentr_value_wrap(result);
}

/* mutator */
void instrumentr_eval_set_result(instrumentr_eval_t eval,
                                 instrumentr_value_t result) {
    eval->result = result;
    instrumentr_value_acquire(result);
}
