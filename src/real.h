#ifndef INSTRUMENTR_REAL_H
#define INSTRUMENTR_REAL_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_real_t instrumentr_real_create(instrumentr_state_t state,
                                                 SEXP r_real);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(real, INSTRUMENTR_MODEL_TYPE_REAL)

INSTRUMENTR_REAL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_REAL_H */