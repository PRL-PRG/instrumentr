#ifndef INSTRUMENTR_VALUE_H
#define INSTRUMENTR_VALUE_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t instrumentr_value_create(instrumentr_state_t state,
                                             SEXP r_value);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(value, INSTRUMENTR_MODEL_TYPE_VALUE)

INSTRUMENTR_VALUE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_VALUE_H */
