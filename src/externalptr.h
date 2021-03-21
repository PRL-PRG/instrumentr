#ifndef INSTRUMENTR_EXTERNALPTR_H
#define INSTRUMENTR_EXTERNALPTR_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_externalptr_t
instrumentr_externalptr_create(instrumentr_state_t state, SEXP r_externalptr);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(externalptr,
                                      INSTRUMENTR_MODEL_TYPE_EXTERNALPTR)

INSTRUMENTR_EXTERNALPTR_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EXTERNALPTR_H */
