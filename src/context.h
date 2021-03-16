#ifndef INSTRUMENTR_CONTEXT_H
#define INSTRUMENTR_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_context_t instrumentr_context_create(instrumentr_state_t state,
                                                 void* pointer);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(context, INSTRUMENTR_MODEL_TYPE_CONTEXT)

/********************************************************************************
 * pointer
 *******************************************************************************/

/* accessor  */
void* instrumentr_context_get_pointer(instrumentr_context_t context);
SEXP r_instrumentr_context_get_pointer(SEXP r_context);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CONTEXT_H */
