#ifndef INSTRUMENTR_CALLBACK_H
#define INSTRUMENTR_CALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/types.h>
#include <instrumentr/Rincludes.h>
#include "object.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_callback_t
instrumentr_callback_create_from_r_function(SEXP r_function,
                                            instrumentr_event_t event);

SEXP r_instrumentr_callback_create_from_r_function(SEXP r_event,
                                                   SEXP r_function);

instrumentr_callback_t
instrumentr_callback_create_from_c_function(void* c_function,
                                            instrumentr_event_t event);

SEXP r_instrumentr_callback_create_from_c_function(SEXP r_c_function,
                                                   SEXP r_event);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DECLARE_API(callback,
                                       INSTRUMENTR_MODEL_TYPE_CALLBACK)

/********************************************************************************
 * event
 *******************************************************************************/

/* accessor  */
instrumentr_event_t
instrumentr_callback_get_event(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_event(SEXP r_callback);

bool instrumentr_callback_handles_event(instrumentr_callback_t callback,
                                        instrumentr_event_t event);
SEXP r_instrumentr_callback_handles_event(SEXP r_callback, SEXP r_event);

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_get_parameter_count(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_parameter_count(SEXP r_callback);

/********************************************************************************
 * r_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_r_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_has_r_function(SEXP r_callback);

/* accessor  */
SEXP instrumentr_callback_get_r_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_r_function(SEXP r_callback);

/********************************************************************************
 * c_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_c_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_has_c_function(SEXP r_callback);

/* accessor  */
void* instrumentr_callback_get_c_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_c_function(SEXP r_callback);

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_is_active(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_is_active(SEXP r_callback);

/* mutator  */
void instrumentr_callback_activate(instrumentr_callback_t callback);

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CALLBACK_H */
