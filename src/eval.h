#ifndef INSTRUMENTR_EVAL_H
#define INSTRUMENTR_EVAL_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_eval_t instrumentr_eval_create(instrumentr_state_t state,
                                           SEXP r_expression,
                                           SEXP r_environment);

/********************************************************************************
 * interop
 *******************************************************************************/

void instrumentr_eval_activate(instrumentr_eval_t eval);

void instrumentr_eval_deactivate(instrumentr_eval_t eval);

void instrumentr_eval_set_result(instrumentr_eval_t eval,
                                 instrumentr_value_t result);

INSTRUMENTR_EVAL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EVAL_H */
