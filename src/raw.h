#ifndef INSTRUMENTR_RAW_H
#define INSTRUMENTR_RAW_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_raw_t instrumentr_raw_create(instrumentr_state_t state, SEXP r_raw);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(raw, INSTRUMENTR_MODEL_TYPE_RAW)

INSTRUMENTR_RAW_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_RAW_H */
