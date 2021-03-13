#ifndef INSTRUMENTR_PROMISE_H
#define INSTRUMENTR_PROMISE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(SEXP r_promise);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_promise_wrap(instrumentr_promise_t promise);

instrumentr_promise_t instrumentr_promise_unwrap(SEXP r_promise);

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
void instrumentr_promise_set_argument(instrumentr_promise_t promise,
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
 * argument
 *******************************************************************************/

/* accessor  */
instrumentr_argument_t
instrumentr_promise_get_argument(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_argument(SEXP r_promise);

/********************************************************************************
 * parameter
 *******************************************************************************/

/* accessor  */
instrumentr_parameter_t
instrumentr_promise_get_parameter(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_parameter(SEXP r_promise);

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
