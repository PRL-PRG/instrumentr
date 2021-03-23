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
 * value
 ******************************************************************************/

instrumentr_value_t instrumentr_state_create_value(instrumentr_state_t state,
                                                   SEXP r_object);

/*******************************************************************************
 * value_table
 *******************************************************************************/

void instrumentr_state_value_table_initialize(instrumentr_state_t state);

instrumentr_value_t
instrumentr_state_value_table_insert(instrumentr_state_t state, SEXP r_value);

void instrumentr_state_value_table_remove(instrumentr_state_t state,
                                          SEXP r_value);

instrumentr_value_t
instrumentr_state_value_table_lookup(instrumentr_state_t state,
                                     SEXP r_value,
                                     int create);

instrumentr_closure_t
instrumentr_state_value_table_lookup_closure(instrumentr_state_t state,
                                             SEXP r_value,
                                             int create);

instrumentr_environment_t
instrumentr_state_value_table_lookup_environment(instrumentr_state_t state,
                                                 SEXP r_value,
                                                 int create);

instrumentr_promise_t
instrumentr_state_value_table_lookup_promise(instrumentr_state_t state,
                                             SEXP r_value,
                                             int create);

void instrumentr_state_value_table_clear(instrumentr_state_t state);

instrumentr_environment_t
instrumentr_state_value_table_lookup_package(instrumentr_state_t state,
                                             const char* package_name);

instrumentr_environment_t
instrumentr_state_value_table_lookup_namespace(instrumentr_state_t state,
                                               const char* package_name);

instrumentr_environment_t
instrumentr_state_value_table_update_namespace(instrumentr_state_t state,
                                               const char* name);

INSTRUMENTR_STATE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_STATE_H */
