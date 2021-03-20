#ifndef INSTRUMENTR_STATE_H
#define INSTRUMENTR_STATE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "object.h"
#include <vector>

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

int instrumentr_state_get_time(instrumentr_state_t state);

void instrumentr_state_increment_time(instrumentr_state_t state);

/*******************************************************************************
 * alloc_stats
 *******************************************************************************/

instrumentr_alloc_stats_t
instrumentr_state_get_alloc_stats(instrumentr_state_t state);
SEXP r_instrumentr_state_get_alloc_stats(SEXP r_state);

/*******************************************************************************
 * exec_stats
 *******************************************************************************/

instrumentr_exec_stats_t
instrumentr_state_get_exec_stats(instrumentr_state_t state);
SEXP r_instrumentr_state_get_exec_stats(SEXP r_state);

/*******************************************************************************
 * empty
 *******************************************************************************/

/*  accessor  */
int instrumentr_state_is_empty(instrumentr_state_t state);
SEXP r_instrumentr_state_is_empty(SEXP r_state);

/*******************************************************************************
 * size
 *******************************************************************************/

/*  accessor  */
int instrumentr_state_get_size(instrumentr_state_t state);
SEXP r_instrumentr_state_get_size(SEXP r_state);

/*******************************************************************************
 * clear
 *******************************************************************************/

/*  mutator  */
void instrumentr_state_clear(instrumentr_state_t state);
SEXP r_instrumentr_state_clear(SEXP r_state);

/*******************************************************************************
 * key
 *******************************************************************************/
/*  accessor  */
int instrumentr_state_has_key(instrumentr_state_t state, const char* key);
SEXP r_instrumentr_state_has_key(SEXP r_state, SEXP r_key);

/*******************************************************************************
 * as_list
 *******************************************************************************/
/*  accessor  */
SEXP instrumentr_state_as_list(instrumentr_state_t state);
SEXP r_instrumentr_state_as_list(SEXP r_state);

/*******************************************************************************
 * lookup
 *******************************************************************************/
/*  accessor  */
SEXP instrumentr_state_lookup(instrumentr_state_t state,
                              const char* key,
                              SEXP r_alternative);
SEXP r_instrumentr_state_lookup(SEXP r_state, SEXP r_key, SEXP r_alternative);

/*******************************************************************************
 * insert
 *******************************************************************************/
/*  mutator  */
void instrumentr_state_insert(instrumentr_state_t state,
                              const char* key,
                              SEXP r_value,
                              int overwrite);
SEXP r_instrumentr_state_insert(SEXP r_state,
                                SEXP r_key,
                                SEXP r_value,
                                SEXP r_overwrite);

/*******************************************************************************
 * erase
 *******************************************************************************/
/*  mutator  */
void instrumentr_state_erase(instrumentr_state_t state,
                             const char* key,
                             int permissive);
SEXP r_instrumentr_state_erase(SEXP r_state, SEXP r_key, SEXP r_permissive);

/*******************************************************************************
 * call_stack
 *******************************************************************************/

/* accessor */
instrumentr_call_stack_t
instrumentr_state_get_call_stack(instrumentr_state_t state);
SEXP r_instrumentr_state_get_call_stack(SEXP r_state);

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

std::vector<instrumentr_environment_t>
instrumentr_state_get_packages(instrumentr_state_t state);

SEXP r_instrumentr_state_get_packages(SEXP r_state);

std::vector<instrumentr_environment_t>
instrumentr_state_get_namespaces(instrumentr_state_t state);

SEXP r_instrumentr_state_get_namespaces(SEXP r_state);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_STATE_H */
