#ifndef INSTRUMENTR_RAW_H
#define INSTRUMENTR_RAW_H

#include <instrumentr/api.h>
#include "value.h"

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

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_RAW, raw, raw)

INSTRUMENTR_RAW_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_RAW_H */
