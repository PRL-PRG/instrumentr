#ifndef INSTRUMENTR_WEAKREF_H
#define INSTRUMENTR_WEAKREF_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_weakref_t instrumentr_weakref_create(instrumentr_state_t state,
                                                 SEXP r_weakref);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_WEAKREF_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_WEAKREF_H */
