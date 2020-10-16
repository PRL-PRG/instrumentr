#ifndef INSTRUMENTR_ARGUMENT_INTERNALS_H
#define INSTRUMENTR_ARGUMENT_INTERNALS_H

#include <instrumentr/argument.h>
#include <instrumentr/Rincludes.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t instrumentr_argument_create(const char* name,
                                                   SEXP r_promise);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_argument_wrap(instrumentr_argument_t argument);

instrumentr_argument_t instrumentr_argument_unwrap(SEXP r_argument);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_has_name(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_has_name(SEXP r_argument);

/* accessor  */
const char* instrumentr_argument_get_name(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_get_name(SEXP r_argument);

/********************************************************************************
 * r_promise
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_evaluated(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_is_evaluated(SEXP r_argument);

/* accessor  */
SEXP instrumentr_argument_get_promise(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_get_promise(SEXP r_argument);

/* accessor  */
SEXP instrumentr_argument_get_expression(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_get_expression(SEXP r_argument);

/* accessor  */
SEXP instrumentr_argument_get_value(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_get_value(SEXP r_argument);

#endif /* INSTRUMENTR_ARGUMENT_INTERNALS_H */
