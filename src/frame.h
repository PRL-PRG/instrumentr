#ifndef INSTRUMENTR_FRAME_H
#define INSTRUMENTR_FRAME_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "call.h"
#include "promise.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_frame_t
instrumentr_frame_create_from_call(instrumentr_call_t call);

instrumentr_frame_t
instrumentr_frame_create_from_promise(instrumentr_promise_t promise);

instrumentr_frame_t
instrumentr_frame_create_from_context(instrumentr_context_t context);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_frame_wrap(instrumentr_frame_t frame);
instrumentr_frame_t instrumentr_frame_unwrap(SEXP r_frame);

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_call(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_is_call(SEXP r_frame);

instrumentr_call_t instrumentr_frame_as_call(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_as_call(SEXP r_frame);

/********************************************************************************
 * promise
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_promise(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_is_promise(SEXP r_frame);

instrumentr_promise_t instrumentr_frame_as_promise(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_as_promise(SEXP r_frame);

/********************************************************************************
 * context
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_context(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_is_context(SEXP r_frame);

instrumentr_context_t instrumentr_frame_as_context(instrumentr_frame_t frame);
SEXP r_instrumentr_frame_as_context(SEXP r_frame);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FRAME_H */
