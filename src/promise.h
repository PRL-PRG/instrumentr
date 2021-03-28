#ifndef INSTRUMENTR_PROMISE_H
#define INSTRUMENTR_PROMISE_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(instrumentr_state_t state,
                                                 SEXP r_sexp);

/********************************************************************************
 * interop
 *******************************************************************************/

void instrumentr_promise_make_argument(instrumentr_promise_t promise,
                                       instrumentr_call_t call);

void instrumentr_promise_set_force_entry_time(instrumentr_promise_t promise,
                                              int force_entry_time);

void instrumentr_promise_set_force_exit_time(instrumentr_promise_t promise,
                                             int force_exit_time);

INSTRUMENTR_PROMISE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PROMISE_H */
