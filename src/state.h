#ifndef INSTRUMENTR_STATE_H
#define INSTRUMENTR_STATE_H

#include <instrumentr/api.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_state_t instrumentr_state_create();

/********************************************************************************
 * finalize tracing
 *******************************************************************************/

SEXP instrumentr_state_finalize_tracing(instrumentr_state_t state);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DECLARE_API(state, INSTRUMENTR_MODEL_TYPE_STATE)

/********************************************************************************
 * id
 *******************************************************************************/

int instrumentr_state_get_next_id(instrumentr_state_t state);

/********************************************************************************
 * time
 *******************************************************************************/

void instrumentr_state_increment_time(instrumentr_state_t state);

/*******************************************************************************
 * promise_table
 *******************************************************************************/
instrumentr_promise_t
instrumentr_state_promise_table_create(instrumentr_state_t state,
                                       SEXP r_promise);

void instrumentr_state_promise_table_remove(instrumentr_state_t state,
                                            SEXP r_promise);

instrumentr_promise_t
instrumentr_state_promise_table_lookup(instrumentr_state_t state,
                                       SEXP r_promise,
                                       int create);

void instrumentr_state_promise_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * function_table
 *******************************************************************************/

instrumentr_function_t
instrumentr_state_function_table_create(instrumentr_state_t state,
                                        SEXP r_function);

void instrumentr_state_function_table_remove(instrumentr_state_t state,
                                             SEXP r_function);

instrumentr_function_t
instrumentr_state_function_table_lookup(instrumentr_state_t state,
                                        SEXP r_function,
                                        int create);

void instrumentr_state_function_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * environment_table
 *******************************************************************************/

void instrumentr_state_environment_table_initialize(instrumentr_state_t state);

instrumentr_environment_t
instrumentr_state_environment_table_create(instrumentr_state_t state,
                                           SEXP r_environment);

void instrumentr_state_environment_table_remove(instrumentr_state_t state,
                                                SEXP r_environment);

instrumentr_environment_t
instrumentr_state_environment_table_lookup(instrumentr_state_t state,
                                           SEXP r_environment,
                                           int create);

void instrumentr_state_environment_table_clear(instrumentr_state_t state);

instrumentr_environment_t
instrumentr_state_environment_table_lookup_package(instrumentr_state_t state,
                                                   const char* package_name);

instrumentr_environment_t
instrumentr_state_environment_table_lookup_namespace(instrumentr_state_t state,
                                                     const char* package_name);

instrumentr_environment_t
instrumentr_state_environment_table_update_namespace(instrumentr_state_t state,
                                                     const char* name);

/*******************************************************************************
 * miscellaneous_table
 *******************************************************************************/

instrumentr_miscellaneous_t
instrumentr_state_miscellaneous_table_create(instrumentr_state_t state,
                                             SEXP r_miscellaneous);

void instrumentr_state_miscellaneous_table_remove(instrumentr_state_t state,
                                                  SEXP r_miscellaneous);

void instrumentr_state_miscellaneous_table_clear(instrumentr_state_t state);

INSTRUMENTR_STATE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_STATE_H */
