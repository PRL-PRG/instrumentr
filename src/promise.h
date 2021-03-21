#ifndef INSTRUMENTR_PROMISE_H
#define INSTRUMENTR_PROMISE_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(instrumentr_state_t state,
                                                 SEXP r_promise);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(promise, INSTRUMENTR_MODEL_TYPE_PROMISE)

void instrumentr_promise_add_call(instrumentr_promise_t promise,
                                  instrumentr_call_t call,
                                  instrumentr_parameter_t parameter,
                                  instrumentr_argument_t argument);

INSTRUMENTR_PROMISE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PROMISE_H */
