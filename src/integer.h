#ifndef INSTRUMENTR_INTEGER_H
#define INSTRUMENTR_INTEGER_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_integer_t instrumentr_integer_create(instrumentr_state_t state,
                                                 SEXP r_integer);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_INTEGER, integer, integer)

INSTRUMENTR_INTEGER_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_INTEGER_H */
