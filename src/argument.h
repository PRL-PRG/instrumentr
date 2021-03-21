#ifndef INSTRUMENTR_ARGUMENT_H
#define INSTRUMENTR_ARGUMENT_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t
instrumentr_argument_create_from_promise(instrumentr_state_t state,
                                         const char* name,
                                         instrumentr_promise_t promise);

instrumentr_argument_t
instrumentr_argument_create_from_value(instrumentr_state_t state,
                                       const char* name,
                                       instrumentr_value_t value);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(argument, INSTRUMENTR_MODEL_TYPE_ARGUMENT)

INSTRUMENTR_ARGUMENT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ARGUMENT_H */
