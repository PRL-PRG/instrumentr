#ifndef INSTRUMENTR_UNBOUND_H
#define INSTRUMENTR_UNBOUND_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_unbound_t instrumentr_unbound_create(instrumentr_state_t state,
                                                 SEXP r_unbound);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(unbound, INSTRUMENTR_MODEL_TYPE_UNBOUND)

INSTRUMENTR_UNBOUND_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_UNBOUND_H */
