#ifndef INSTRUMENTR_FUNCTION_H
#define INSTRUMENTR_FUNCTION_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t instrumentr_function_create(instrumentr_state_t state,
                                                   SEXP r_function);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(function, INSTRUMENTR_MODEL_TYPE_FUNCTION)

/********************************************************************************
 * type
 *******************************************************************************/

/* TODO add R funciton for get type. Add a type table similar to model_type */

void instrumentr_function_set_name(instrumentr_function_t function,
                                   const char* name);

void instrumentr_function_set_environment(instrumentr_function_t function);

void instrumentr_function_set_exported(instrumentr_function_t function);

void instrumentr_function_set_object_class(instrumentr_function_t function,
                                           const char* object_class);

void instrumentr_function_set_generic_name(instrumentr_function_t function,
                                           const char* generic_name);

INSTRUMENTR_FUNCTION_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FUNCTION_H */
