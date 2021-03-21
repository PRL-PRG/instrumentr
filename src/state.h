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

/*******************************************************************************
 * unbound_table
 *******************************************************************************/

instrumentr_unbound_t
instrumentr_state_unbound_table_create(instrumentr_state_t state,
                                       SEXP r_unbound);

void instrumentr_state_unbound_table_remove(instrumentr_state_t state,
                                            SEXP r_unbound);
instrumentr_unbound_t
instrumentr_state_unbound_table_lookup(instrumentr_state_t state,
                                       SEXP r_unbound,
                                       int create);

void instrumentr_state_unbound_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * missing_table
 *******************************************************************************/

instrumentr_missing_t
instrumentr_state_missing_table_create(instrumentr_state_t state,
                                       SEXP r_missing);

void instrumentr_state_missing_table_remove(instrumentr_state_t state,
                                            SEXP r_missing);
instrumentr_missing_t
instrumentr_state_missing_table_lookup(instrumentr_state_t state,
                                       SEXP r_missing,
                                       int create);

void instrumentr_state_missing_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * null_table
 *******************************************************************************/

instrumentr_null_t
instrumentr_state_null_table_create(instrumentr_state_t state, SEXP r_null);

void instrumentr_state_null_table_remove(instrumentr_state_t state,
                                         SEXP r_null);
instrumentr_null_t
instrumentr_state_null_table_lookup(instrumentr_state_t state,
                                    SEXP r_null,
                                    int create);

void instrumentr_state_null_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * externalptr_table
 *******************************************************************************/

instrumentr_externalptr_t
instrumentr_state_externalptr_table_create(instrumentr_state_t state,
                                           SEXP r_externalptr);

void instrumentr_state_externalptr_table_remove(instrumentr_state_t state,
                                                SEXP r_externalptr);
instrumentr_externalptr_t
instrumentr_state_externalptr_table_lookup(instrumentr_state_t state,
                                           SEXP r_externalptr,
                                           int create);

void instrumentr_state_externalptr_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * weakref_table
 *******************************************************************************/

instrumentr_weakref_t
instrumentr_state_weakref_table_create(instrumentr_state_t state,
                                       SEXP r_weakref);

void instrumentr_state_weakref_table_remove(instrumentr_state_t state,
                                            SEXP r_weakref);
instrumentr_weakref_t
instrumentr_state_weakref_table_lookup(instrumentr_state_t state,
                                       SEXP r_weakref,
                                       int create);

void instrumentr_state_weakref_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * bytecode_table
 *******************************************************************************/

instrumentr_bytecode_t
instrumentr_state_bytecode_table_create(instrumentr_state_t state,
                                        SEXP r_bytecode);

void instrumentr_state_bytecode_table_remove(instrumentr_state_t state,
                                             SEXP r_bytecode);
instrumentr_bytecode_t
instrumentr_state_bytecode_table_lookup(instrumentr_state_t state,
                                        SEXP r_bytecode,
                                        int create);

void instrumentr_state_bytecode_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * s4_table
 *******************************************************************************/

instrumentr_s4_t instrumentr_state_s4_table_create(instrumentr_state_t state,
                                                   SEXP r_s4);

void instrumentr_state_s4_table_remove(instrumentr_state_t state, SEXP r_s4);
instrumentr_s4_t instrumentr_state_s4_table_lookup(instrumentr_state_t state,
                                                   SEXP r_s4,
                                                   int create);

void instrumentr_state_s4_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * char_table
 *******************************************************************************/

instrumentr_char_t
instrumentr_state_char_table_create(instrumentr_state_t state, SEXP r_char);

void instrumentr_state_char_table_remove(instrumentr_state_t state,
                                         SEXP r_char);
instrumentr_char_t
instrumentr_state_char_table_lookup(instrumentr_state_t state,
                                    SEXP r_char,
                                    int create);

void instrumentr_state_char_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * symbol_table
 *******************************************************************************/

instrumentr_symbol_t
instrumentr_state_symbol_table_create(instrumentr_state_t state, SEXP r_symbol);

void instrumentr_state_symbol_table_remove(instrumentr_state_t state,
                                           SEXP r_symbol);

void instrumentr_state_symbol_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * character_table
 *******************************************************************************/

instrumentr_character_t
instrumentr_state_character_table_create(instrumentr_state_t state,
                                         SEXP r_character);

void instrumentr_state_character_table_remove(instrumentr_state_t state,
                                              SEXP r_character);

void instrumentr_state_character_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * integer_table
 *******************************************************************************/

instrumentr_integer_t
instrumentr_state_integer_table_create(instrumentr_state_t state,
                                       SEXP r_integer);

void instrumentr_state_integer_table_remove(instrumentr_state_t state,
                                            SEXP r_integer);

void instrumentr_state_integer_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * real_table
 *******************************************************************************/

instrumentr_real_t
instrumentr_state_real_table_create(instrumentr_state_t state, SEXP r_real);

void instrumentr_state_real_table_remove(instrumentr_state_t state,
                                         SEXP r_real);

void instrumentr_state_real_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * logical_table
 *******************************************************************************/

instrumentr_logical_t
instrumentr_state_logical_table_create(instrumentr_state_t state,
                                       SEXP r_logical);

void instrumentr_state_logical_table_remove(instrumentr_state_t state,
                                            SEXP r_logical);

void instrumentr_state_logical_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * complex_table
 *******************************************************************************/

instrumentr_complex_t
instrumentr_state_complex_table_create(instrumentr_state_t state,
                                       SEXP r_complex);

void instrumentr_state_complex_table_remove(instrumentr_state_t state,
                                            SEXP r_complex);

void instrumentr_state_complex_table_clear(instrumentr_state_t state);

/*******************************************************************************
 * raw_table
 *******************************************************************************/

instrumentr_raw_t instrumentr_state_raw_table_create(instrumentr_state_t state,
                                                     SEXP r_raw);

void instrumentr_state_raw_table_remove(instrumentr_state_t state, SEXP r_raw);

void instrumentr_state_raw_table_clear(instrumentr_state_t state);

INSTRUMENTR_STATE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_STATE_H */
