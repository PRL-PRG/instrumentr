#ifndef INSTRUMENTR_DOT_H
#define INSTRUMENTR_DOT_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_dot_t instrumentr_dot_create(instrumentr_state_t state, SEXP r_dot);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_DOT, dot, dot)

INSTRUMENTR_DOT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_DOT_H */
