#ifndef INSTRUMENTR_PROMISE_H
#define INSTRUMENTR_PROMISE_H

#include <vector>
#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(instrumentr_state_t state,
                                                 SEXP r_promise);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(promise, INSTRUMENTR_MODEL_TYPE_PROMISE)

/********************************************************************************
 * forced
 *******************************************************************************/

/* accessor  */
int instrumentr_promise_is_forced(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_forced(SEXP r_promise);

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_promise_get_expression(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_expression(SEXP r_promise);

/********************************************************************************
 * value
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_promise_get_value(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_value(SEXP r_promise);

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_promise_type_t
instrumentr_promise_get_type(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_type(SEXP r_promise);

/* accessor  */
int instrumentr_promise_is_argument(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_argument(SEXP r_promise);

/* mutator */
void instrumentr_promise_add_call(instrumentr_promise_t promise,
                                  instrumentr_call_t call,
                                  instrumentr_parameter_t parameter,
                                  instrumentr_argument_t argument);

/* accessor  */
int instrumentr_promise_is_delayed_assign(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_delayed_assign(SEXP r_promise);

/* accessor  */
int instrumentr_promise_is_lazy_load(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_lazy_load(SEXP r_promise);

/* accessor  */
int instrumentr_promise_is_unknown(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_unknown(SEXP r_promise);

/********************************************************************************
 * call_info_seq
 *******************************************************************************/

const std::vector<instrumentr_promise_call_info_t>&
instrumentr_promise_get_call_info(instrumentr_promise_t promise);

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
instrumentr_call_t instrumentr_promise_get_call(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_call(SEXP r_promise);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PROMISE_H */
