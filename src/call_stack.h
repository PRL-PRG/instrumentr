#ifndef INSTRUMENTR_CALL_STACK_H
#define INSTRUMENTR_CALL_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t
instrumentr_call_stack_create(instrumentr_state_t state);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(call_stack,
                                      INSTRUMENTR_MODEL_TYPE_CALL_STACK)

/********************************************************************************
 * size
 *******************************************************************************/

/* accessor  */
int instrumentr_call_stack_get_size(instrumentr_call_stack_t call_stack);
SEXP r_instrumentr_call_stack_get_size(SEXP r_call_stack);

/********************************************************************************
 * calls
 *******************************************************************************/

/* mutator  */
void instrumentr_call_stack_push_frame(instrumentr_call_stack_t call_stack,
                                       instrumentr_frame_t frame);

/* mutator  */
void instrumentr_call_stack_pop_frame(instrumentr_call_stack_t call_stack);

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_get_frame(instrumentr_call_stack_t call_stack,
                                 int index);
SEXP r_instrumentr_call_stack_get_frame(SEXP r_call_stack, SEXP r_index);

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_peek_frame(instrumentr_call_stack_t call_stack,
                                  int index);
SEXP r_instrumentr_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index);

/********************************************************************************
 * jumped
 *******************************************************************************/

int instrumentr_call_stack_get_jumped_frame_count(
    instrumentr_call_stack_t call_stack,
    void* pointer);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CALL_STACK_H */
