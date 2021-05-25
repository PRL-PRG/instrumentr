#ifndef INSTRUMENTR_FRAME_H
#define INSTRUMENTR_FRAME_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/
instrumentr_frame_t
instrumentr_frame_create_from_eval(instrumentr_state_t state,
                                   instrumentr_eval_t eval);

instrumentr_frame_t
instrumentr_frame_create_from_call(instrumentr_state_t state,
                                   instrumentr_call_t call);

instrumentr_frame_t
instrumentr_frame_create_from_promise(instrumentr_state_t state,
                                      instrumentr_promise_t promise);

instrumentr_frame_t
instrumentr_frame_create_from_context(instrumentr_state_t state,
                                      instrumentr_context_t context);

INSTRUMENTR_FRAME_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FRAME_H */
