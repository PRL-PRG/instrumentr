#ifndef INSTRUMENTR_ARGUMENT_H
#define INSTRUMENTR_ARGUMENT_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "value.h"
#include "promise.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t instrumentr_argument_create(const char* name,
                                                   instrumentr_object_t object);

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
 * promise
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_promise(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_is_promise(SEXP r_argument);

instrumentr_promise_t
instrumentr_argument_as_promise(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_as_promise(SEXP r_argument);

/********************************************************************************
 * value
 *******************************************************************************/

/* accessor  */
int instrumentr_argument_is_value(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_is_value(SEXP r_argument);

instrumentr_value_t
instrumentr_argument_as_value(instrumentr_argument_t argument);
SEXP r_instrumentr_argument_as_value(SEXP r_argument);

#endif /* INSTRUMENTR_ARGUMENT_H */
