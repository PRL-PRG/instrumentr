#ifndef INSTRUMENTR_CALLBACK_H
#define INSTRUMENTR_CALLBACK_H

#include <instrumentr/api.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DECLARE_API(callback,
                                       INSTRUMENTR_OBJECT_TYPE_CALLBACK)

/* mutator  */
void instrumentr_callback_activate(instrumentr_callback_t callback);

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback);

INSTRUMENTR_CALLBACK_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CALLBACK_H */
