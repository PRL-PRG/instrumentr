#ifndef INSTRUMENTR_ENVIRONMENT_H
#define INSTRUMENTR_ENVIRONMENT_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_environment_t
instrumentr_environment_create(instrumentr_state_t state, SEXP r_environment);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(environment,
                                      INSTRUMENTR_MODEL_TYPE_ENVIRONMENT)

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_environment_type_t
instrumentr_environment_get_type(instrumentr_environment_t environment);
/* TODO: add R API */

void instrumentr_environment_set_type(instrumentr_environment_t environment,
                                      instrumentr_environment_type_t type);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_environment_get_name(instrumentr_environment_t environment);
SEXP r_instrumentr_environment_get_name(SEXP r_environment);

/* mutator */
void instrumentr_environment_set_name(instrumentr_environment_t environment,
                                      const char* name);

/********************************************************************************
 * r_environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_environment_get_environment(
    instrumentr_environment_t environment);
SEXP r_instrumentr_environment_get_environment(SEXP r_environment);

/********************************************************************************
 * bindings
 *******************************************************************************/

/* accessor  */
int instrumentr_environment_get_size(instrumentr_environment_t environment);
SEXP r_instrumentr_environment_get_size(SEXP r_environment);

/* accessor  */
instrumentr_function_t
instrumentr_environment_lookup(instrumentr_environment_t environment,
                               const char* name);
SEXP r_instrumentr_environment_lookup(SEXP r_environment, SEXP r_name);

/* accessor */
int instrumentr_environment_contains(instrumentr_environment_t environment,
                                     const char* name);
SEXP r_instrumentr_environment_contains(SEXP r_environment, SEXP r_name);

/* accessor  */
void instrumentr_environment_insert(instrumentr_environment_t environment,
                                    const char* name,
                                    instrumentr_function_t function);

/* accessor  */
const std::unordered_map<std::string, instrumentr_function_t>&
instrumentr_environment_get_bindings(instrumentr_environment_t environment);
/* accessor  */
SEXP r_instrumentr_environment_get_bindings(SEXP r_environment);

/* mutator */
void instrumentr_environment_clear(instrumentr_environment_t environment);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ENVIRONMENT_H */
