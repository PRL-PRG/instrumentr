#ifndef INSTRUMENTR_NULL_H
#define INSTRUMENTR_NULL_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_null_t instrumentr_null_create(instrumentr_state_t state,
                                           SEXP r_null);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(null, INSTRUMENTR_MODEL_TYPE_NULL)

INSTRUMENTR_NULL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_NULL_H */
