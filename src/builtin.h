#ifndef INSTRUMENTR_BUILTIN_H
#define INSTRUMENTR_BUILTIN_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_builtin_t instrumentr_builtin_create(instrumentr_state_t state,
                                                 SEXP r_sexp);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(builtin, INSTRUMENTR_MODEL_TYPE_BUILTIN)

INSTRUMENTR_BUILTIN_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_BUILTIN_H */
