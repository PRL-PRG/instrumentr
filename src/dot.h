#ifndef INSTRUMENTR_DOT_H
#define INSTRUMENTR_DOT_H

#include <instrumentr/api.h>
#include "model.h"

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

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(dot, INSTRUMENTR_MODEL_TYPE_DOT)

INSTRUMENTR_DOT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_DOT_H */
