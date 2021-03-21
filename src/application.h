#ifndef INSTRUMENTR_APPLICATION_H
#define INSTRUMENTR_APPLICATION_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_application_t
instrumentr_application_create(instrumentr_state_t state,
                               const char* const directory,
                               SEXP r_code,
                               instrumentr_environment_t environment);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(application,
                                      INSTRUMENTR_MODEL_TYPE_APPLICATION)

INSTRUMENTR_APPLICATION_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_APPLICATION_H */
