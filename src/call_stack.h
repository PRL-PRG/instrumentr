#ifndef INSTRUMENTR_CALL_STACK_H
#define INSTRUMENTR_CALL_STACK_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t instrumentr_call_stack_create();

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_call_stack_wrap(instrumentr_call_stack_t call_stack);

instrumentr_call_stack_t instrumentr_call_stack_unwrap(SEXP r_call_stack);

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
void instrumentr_call_stack_push(instrumentr_call_stack_t call_stack,
                                 instrumentr_frame_t frame);

/* mutator  */
void instrumentr_call_stack_pop(instrumentr_call_stack_t call_stack);

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_peek_frame(instrumentr_call_stack_t call_stack,
                                  int index);
SEXP r_instrumentr_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index);


#endif /* INSTRUMENTR_CALL_STACK_H */
