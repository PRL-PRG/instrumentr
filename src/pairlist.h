#ifndef INSTRUMENTR_PAIRLIST_H
#define INSTRUMENTR_PAIRLIST_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_pairlist_t instrumentr_pairlist_create(instrumentr_state_t state,
                                                   SEXP r_pairlist);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_PAIRLIST,
                              pairlist,
                              pairlist)

INSTRUMENTR_PAIRLIST_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PAIRLIST_H */
