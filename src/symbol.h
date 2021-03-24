#ifndef INSTRUMENTR_SYMBOL_H
#define INSTRUMENTR_SYMBOL_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_symbol_t instrumentr_symbol_create(instrumentr_state_t state,
                                               SEXP r_symbol);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_SYMBOL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_SYMBOL_H */
