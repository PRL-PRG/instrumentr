#ifndef INSTRUMENTR_LOGICAL_H
#define INSTRUMENTR_LOGICAL_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_logical_t instrumentr_logical_create(instrumentr_state_t state,
                                                 SEXP r_logical);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_LOGICAL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_LOGICAL_H */
