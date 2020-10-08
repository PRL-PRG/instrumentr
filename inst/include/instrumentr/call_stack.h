#ifndef INSTRUMENTR_CALL_STACK_H
#define INSTRUMENTR_CALL_STACK_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/call.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_call_stack_impl_t* instrumentr_call_stack_t;

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
                                 instrumentr_call_t call);

/* mutator  */
void instrumentr_call_stack_pop(instrumentr_call_stack_t call_stack);

/* accessor  */
instrumentr_call_t
instrumentr_call_stack_peek(instrumentr_call_stack_t call_stack, int index);
SEXP r_instrumentr_call_stack_peek(SEXP r_call_stack, SEXP r_index);

#endif /* INSTRUMENTR_CALL_STACK_H */
