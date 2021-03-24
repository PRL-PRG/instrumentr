#ifndef INSTRUMENTR_CHAR_H
#define INSTRUMENTR_CHAR_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_char_t instrumentr_char_create(instrumentr_state_t state,
                                           SEXP r_char);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_CHAR_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CHAR_H */
