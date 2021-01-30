#ifndef INSTRUMENTR_VALUE_H
#define INSTRUMENTR_VALUE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t instrumentr_value_create(SEXP r_value);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_value_wrap(instrumentr_value_t value);

instrumentr_value_t instrumentr_value_unwrap(SEXP r_value);

/********************************************************************************
 * r_sexp
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_value_get_sexp(instrumentr_value_t value);
SEXP r_instrumentr_value_get_sexp(SEXP r_value);

/* accessor  */
const char* instrumentr_value_get_sexp_address(instrumentr_value_t value);
SEXP r_instrumentr_value_get_sexp_address(SEXP r_value);

/* accessor  */
SEXPTYPE instrumentr_value_get_sexp_type(instrumentr_value_t value);
SEXP r_instrumentr_value_get_sexp_type(SEXP r_value);

#endif /* INSTRUMENTR_VALUE_H */
