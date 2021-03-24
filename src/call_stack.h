#ifndef INSTRUMENTR_CALL_STACK_H
#define INSTRUMENTR_CALL_STACK_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t
instrumentr_call_stack_create(instrumentr_state_t state);

/********************************************************************************
 * interop
 *******************************************************************************/

void instrumentr_call_stack_push_frame(instrumentr_call_stack_t call_stack,
                                       instrumentr_frame_t frame);

void instrumentr_call_stack_pop_frame(instrumentr_call_stack_t call_stack);

int instrumentr_call_stack_get_jumped_frame_count(
    instrumentr_call_stack_t call_stack,
    void* pointer);

INSTRUMENTR_CALL_STACK_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CALL_STACK_H */
