#ifndef INSTRUMENTR_SPECIAL_H
#define INSTRUMENTR_SPECIAL_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_special_t instrumentr_special_create(instrumentr_state_t state,
                                                 SEXP r_sexp);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(special, INSTRUMENTR_MODEL_TYPE_SPECIAL)

INSTRUMENTR_SPECIAL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_SPECIAL_H */
