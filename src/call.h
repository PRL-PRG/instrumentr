#ifndef INSTRUMENTR_CALL_H
#define INSTRUMENTR_CALL_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_t instrumentr_call_create(instrumentr_state_t state,
                                           instrumentr_value_t function,
                                           SEXP r_expression,
                                           SEXP r_environment);

/********************************************************************************
 * interop
 *******************************************************************************/

void instrumentr_call_activate(instrumentr_call_t call);


void instrumentr_call_deactivate(instrumentr_call_t call);


void instrumentr_call_set_result(instrumentr_call_t call, SEXP r_result);


void instrumentr_call_append_parameter(instrumentr_call_t call,
                                       instrumentr_parameter_t parameter);

INSTRUMENTR_CALL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CALL_H */
