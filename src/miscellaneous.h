#ifndef INSTRUMENTR_MISCELLANEOUS_H
#define INSTRUMENTR_MISCELLANEOUS_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_miscellaneous_t
instrumentr_miscellaneous_create(instrumentr_state_t state,
                                 SEXP r_miscellaneous);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_MISCELLANEOUS,
                              miscellaneous,
                              miscellaneous)

INSTRUMENTR_MISCELLANEOUS_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MISCELLANEOUS_H */
