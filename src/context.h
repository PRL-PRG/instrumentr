#ifndef INSTRUMENTR_CONTEXT_H
#define INSTRUMENTR_CONTEXT_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_context_t instrumentr_context_create(void* pointer);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_context_wrap(instrumentr_context_t context);
instrumentr_context_t instrumentr_context_unwrap(SEXP r_context);

/********************************************************************************
 * pointer
 *******************************************************************************/

/* accessor  */
void* instrumentr_context_get_pointer(instrumentr_context_t context);
SEXP r_instrumentr_context_get_pointer(SEXP r_context);

#endif /* INSTRUMENTR_CONTEXT_H */
