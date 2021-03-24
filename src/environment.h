#ifndef INSTRUMENTR_ENVIRONMENT_H
#define INSTRUMENTR_ENVIRONMENT_H

#include <instrumentr/api.h>
#include "value.h"
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
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_environment_type_t
instrumentr_environment_get_type(instrumentr_environment_t environment);
/* TODO: add R API */

void instrumentr_environment_set_type(instrumentr_environment_t environment,
                                      instrumentr_environment_type_t type);

void instrumentr_environment_set_name(instrumentr_environment_t environment,
                                      const char* name);

void instrumentr_environment_insert(instrumentr_environment_t environment,
                                    const char* name,
                                    instrumentr_closure_t closure);

const std::unordered_map<std::string, instrumentr_closure_t>&
instrumentr_environment_get_bindings(instrumentr_environment_t environment);

void instrumentr_environment_clear(instrumentr_environment_t environment);

void instrumentr_environment_set_call(instrumentr_environment_t environment,
                                      instrumentr_call_t call);

void instrumentr_environment_set_package(instrumentr_environment_t environment,
                                         const char* name);

void instrumentr_environment_set_namespace(
    instrumentr_environment_t environment,
    const char* name);

INSTRUMENTR_ENVIRONMENT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ENVIRONMENT_H */
