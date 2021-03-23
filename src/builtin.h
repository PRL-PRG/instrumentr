#ifndef INSTRUMENTR_BUILTIN_H
#define INSTRUMENTR_BUILTIN_H

#include <instrumentr/api.h>
#include "value.h"

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

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_BUILTIN, builtin, builtin)

INSTRUMENTR_BUILTIN_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_BUILTIN_H */
