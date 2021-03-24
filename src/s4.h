#ifndef INSTRUMENTR_S4_H
#define INSTRUMENTR_S4_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_s4_t instrumentr_s4_create(instrumentr_state_t state, SEXP r_s4);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_S4_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_S4_H */
