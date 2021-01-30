#ifndef INSTRUMENTR_PROMISE_H
#define INSTRUMENTR_PROMISE_H

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
 * name
 *******************************************************************************/

/* accessor  */
int instrumentr_promise_is_forced(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_is_forced(SEXP r_promise);

/* accessor  */
SEXP instrumentr_promise_get_expression(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_expression(SEXP r_promise);

/* accessor  */
SEXP instrumentr_promise_get_value(instrumentr_promise_t promise);
SEXP r_instrumentr_promise_get_value(SEXP r_promise);

#endif /* INSTRUMENTR_PROMISE_H */
