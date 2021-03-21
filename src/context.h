#ifndef INSTRUMENTR_CONTEXT_H
#define INSTRUMENTR_CONTEXT_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_context_t instrumentr_context_create(instrumentr_state_t state,
                                                 void* pointer);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(context, INSTRUMENTR_MODEL_TYPE_CONTEXT)

INSTRUMENTR_CONTEXT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CONTEXT_H */
