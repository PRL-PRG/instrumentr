#ifndef INSTRUMENTR_VALUE_H
#define INSTRUMENTR_VALUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t instrumentr_value_create(instrumentr_state_t state,
                                             SEXP r_value);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(value, INSTRUMENTR_MODEL_TYPE_VALUE)

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

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_VALUE_H */
