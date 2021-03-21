#ifndef INSTRUMENTR_PARAMETER_H
#define INSTRUMENTR_PARAMETER_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(instrumentr_state_t state,
                                                     const char* name,
                                                     int position,
                                                     SEXP r_default_expression);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(parameter,
                                      INSTRUMENTR_MODEL_TYPE_PARAMETER)

void instrumentr_parameter_append_argument(instrumentr_parameter_t parameter,
                                           instrumentr_argument_t argument);

INSTRUMENTR_PARAMETER_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PARAMETER_H */
