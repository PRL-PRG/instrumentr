#include <instrumentr/Rincludes.h>
#include "object.h"
#include "state.h"
#include "interop.h"
#include <string>
#include <unordered_map>
#include "promise.h"
#include "function.h"
#include "call_stack.h"
#include "frame.h"
#include "call.h"
#include "environment.h"
#include "alloc_stats.h"
#include "exec_stats.h"
#include "miscellaneous.h"
#include "unbound.h"
#include "missing.h"
#include "null.h"
#include "externalptr.h"
#include "weakref.h"
#include "bytecode.h"
#include "s4.h"
#include "char.h"
#include "symbol.h"
#include "character.h"
#include "integer.h"
#include "real.h"
#include "logical.h"
#include "complex.h"
#include "raw.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_state_impl_t {
    struct instrumentr_object_impl_t object;

    instrumentr_alloc_stats_t alloc_stats;
    instrumentr_exec_stats_t exec_stats;

    int next_id;
    int time;
    std::unordered_map<std::string, SEXP>* external;
    instrumentr_call_stack_t call_stack;
    std::unordered_map<SEXP, instrumentr_promise_t>* promise_table;
    std::unordered_map<SEXP, instrumentr_function_t>* function_table;
    std::unordered_map<SEXP, instrumentr_environment_t>* environment_table;
    std::unordered_map<SEXP, instrumentr_miscellaneous_t>* miscellaneous_table;

    std::unordered_map<SEXP, instrumentr_unbound_t>* unbound_table;
    std::unordered_map<SEXP, instrumentr_missing_t>* missing_table;
    std::unordered_map<SEXP, instrumentr_null_t>* null_table;
    std::unordered_map<SEXP, instrumentr_externalptr_t>* externalptr_table;
    std::unordered_map<SEXP, instrumentr_weakref_t>* weakref_table;
    std::unordered_map<SEXP, instrumentr_bytecode_t>* bytecode_table;
    std::unordered_map<SEXP, instrumentr_s4_t>* s4_table;
    std::unordered_map<SEXP, instrumentr_char_t>* char_table;
    std::unordered_map<SEXP, instrumentr_symbol_t>* symbol_table;
    std::unordered_map<SEXP, instrumentr_character_t>* character_table;
    std::unordered_map<SEXP, instrumentr_integer_t>* integer_table;
    std::unordered_map<SEXP, instrumentr_real_t>* real_table;
    std::unordered_map<SEXP, instrumentr_logical_t>* logical_table;
    std::unordered_map<SEXP, instrumentr_complex_t>* complex_table;
    std::unordered_map<SEXP, instrumentr_raw_t>* raw_table;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_state_finalize(instrumentr_object_t object) {
    instrumentr_state_t state = (instrumentr_state_t)(object);

    if (state->external != nullptr) {
        instrumentr_state_clear(state);
        delete state->external;
        state->external = nullptr;
    }

    if (state->call_stack != nullptr) {
        instrumentr_model_kill(state->call_stack);
        state->call_stack = nullptr;
    }

    if (state->promise_table != nullptr) {
        instrumentr_state_promise_table_clear(state);
        delete state->promise_table;
        state->promise_table = nullptr;
    }

    if (state->function_table != nullptr) {
        instrumentr_state_function_table_clear(state);
        delete state->function_table;
        state->function_table = nullptr;
    }

    if (state->environment_table != nullptr) {
        instrumentr_state_environment_table_clear(state);
        delete state->environment_table;
        state->environment_table = nullptr;
    }

    if (state->miscellaneous_table != nullptr) {
        instrumentr_state_miscellaneous_table_clear(state);
        delete state->miscellaneous_table;
        state->miscellaneous_table = nullptr;
    }

    if (state->unbound_table != nullptr) {
        instrumentr_state_unbound_table_clear(state);
        delete state->unbound_table;
        state->unbound_table = nullptr;
    }

    if (state->missing_table != nullptr) {
        instrumentr_state_missing_table_clear(state);
        delete state->missing_table;
        state->missing_table = nullptr;
    }

    if (state->null_table != nullptr) {
        instrumentr_state_null_table_clear(state);
        delete state->null_table;
        state->null_table = nullptr;
    }

    if (state->externalptr_table != nullptr) {
        instrumentr_state_externalptr_table_clear(state);
        delete state->externalptr_table;
        state->externalptr_table = nullptr;
    }

    if (state->weakref_table != nullptr) {
        instrumentr_state_weakref_table_clear(state);
        delete state->weakref_table;
        state->weakref_table = nullptr;
    }

    if (state->bytecode_table != nullptr) {
        instrumentr_state_bytecode_table_clear(state);
        delete state->bytecode_table;
        state->bytecode_table = nullptr;
    }

    if (state->s4_table != nullptr) {
        instrumentr_state_s4_table_clear(state);
        delete state->s4_table;
        state->s4_table = nullptr;
    }

    if (state->char_table != nullptr) {
        instrumentr_state_char_table_clear(state);
        delete state->char_table;
        state->char_table = nullptr;
    }

    if (state->symbol_table != nullptr) {
        instrumentr_state_symbol_table_clear(state);
        delete state->symbol_table;
        state->symbol_table = nullptr;
    }

    if (state->character_table != nullptr) {
        instrumentr_state_character_table_clear(state);
        delete state->character_table;
        state->character_table = nullptr;
    }

    if (state->integer_table != nullptr) {
        instrumentr_state_integer_table_clear(state);
        delete state->integer_table;
        state->integer_table = nullptr;
    }

    if (state->real_table != nullptr) {
        instrumentr_state_real_table_clear(state);
        delete state->real_table;
        state->real_table = nullptr;
    }

    if (state->logical_table != nullptr) {
        instrumentr_state_logical_table_clear(state);
        delete state->logical_table;
        state->logical_table = nullptr;
    }

    if (state->complex_table != nullptr) {
        instrumentr_state_complex_table_clear(state);
        delete state->complex_table;
        state->complex_table = nullptr;
    }

    if (state->raw_table != nullptr) {
        instrumentr_state_raw_table_clear(state);
        delete state->raw_table;
        state->raw_table = nullptr;
    }

    instrumentr_object_release(state->alloc_stats);
    instrumentr_object_release(state->exec_stats);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_state_t instrumentr_state_create() {
    instrumentr_state_t state = (instrumentr_state_t) instrumentr_object_create(
        sizeof(instrumentr_state_impl_t),
        INSTRUMENTR_OBJECT_TYPE_STATE,
        instrumentr_state_finalize);

    state->next_id = 0;
    state->time = -1;

    /* NOTE: stats should be created before model objects are allocated. */
    state->alloc_stats = instrumentr_alloc_stats_create();

    state->exec_stats = instrumentr_exec_stats_create();

    state->external = new std::unordered_map<std::string, SEXP>();

    state->promise_table =
        new std::unordered_map<SEXP, instrumentr_promise_t>();
    state->promise_table->reserve(250000);

    state->function_table =
        new std::unordered_map<SEXP, instrumentr_function_t>();
    state->function_table->reserve(250000);

    state->environment_table =
        new std::unordered_map<SEXP, instrumentr_environment_t>();
    state->environment_table->reserve(250000);

    state->miscellaneous_table =
        new std::unordered_map<SEXP, instrumentr_miscellaneous_t>();
    state->miscellaneous_table->reserve(250000);

    state->unbound_table =
        new std::unordered_map<SEXP, instrumentr_unbound_t>();
    state->unbound_table->reserve(250000);

    state->missing_table =
        new std::unordered_map<SEXP, instrumentr_missing_t>();
    state->missing_table->reserve(250000);

    state->null_table = new std::unordered_map<SEXP, instrumentr_null_t>();
    state->null_table->reserve(250000);

    state->externalptr_table =
        new std::unordered_map<SEXP, instrumentr_externalptr_t>();
    state->externalptr_table->reserve(250000);

    state->weakref_table =
        new std::unordered_map<SEXP, instrumentr_weakref_t>();
    state->weakref_table->reserve(250000);

    state->bytecode_table =
        new std::unordered_map<SEXP, instrumentr_bytecode_t>();
    state->bytecode_table->reserve(250000);

    state->s4_table = new std::unordered_map<SEXP, instrumentr_s4_t>();
    state->s4_table->reserve(250000);

    state->char_table = new std::unordered_map<SEXP, instrumentr_char_t>();
    state->char_table->reserve(250000);

    state->symbol_table = new std::unordered_map<SEXP, instrumentr_symbol_t>();
    state->symbol_table->reserve(250000);

    state->character_table =
        new std::unordered_map<SEXP, instrumentr_character_t>();
    state->character_table->reserve(250000);

    state->integer_table =
        new std::unordered_map<SEXP, instrumentr_integer_t>();
    state->integer_table->reserve(250000);

    state->real_table = new std::unordered_map<SEXP, instrumentr_real_t>();
    state->real_table->reserve(250000);

    state->logical_table =
        new std::unordered_map<SEXP, instrumentr_logical_t>();
    state->logical_table->reserve(250000);

    state->complex_table =
        new std::unordered_map<SEXP, instrumentr_complex_t>();
    state->complex_table->reserve(250000);

    state->raw_table = new std::unordered_map<SEXP, instrumentr_raw_t>();
    state->raw_table->reserve(250000);

    state->call_stack = instrumentr_call_stack_create(state);

    return state;
}

/********************************************************************************
 * finalize tracing
 *******************************************************************************/

SEXP instrumentr_state_finalize_tracing(instrumentr_state_t state) {
    instrumentr_model_kill(state->call_stack);
    state->call_stack = nullptr;

    instrumentr_state_promise_table_clear(state);
    delete state->promise_table;
    state->promise_table = nullptr;

    instrumentr_state_function_table_clear(state);
    delete state->function_table;
    state->function_table = nullptr;

    instrumentr_state_environment_table_clear(state);
    delete state->environment_table;
    state->environment_table = nullptr;

    instrumentr_state_miscellaneous_table_clear(state);
    delete state->miscellaneous_table;
    state->miscellaneous_table = nullptr;

    instrumentr_state_unbound_table_clear(state);
    delete state->unbound_table;
    state->unbound_table = nullptr;

    instrumentr_state_missing_table_clear(state);
    delete state->missing_table;
    state->missing_table = nullptr;

    instrumentr_state_null_table_clear(state);
    delete state->null_table;
    state->null_table = nullptr;

    instrumentr_state_externalptr_table_clear(state);
    delete state->externalptr_table;
    state->externalptr_table = nullptr;

    instrumentr_state_weakref_table_clear(state);
    delete state->weakref_table;
    state->weakref_table = nullptr;

    instrumentr_state_bytecode_table_clear(state);
    delete state->bytecode_table;
    state->bytecode_table = nullptr;

    instrumentr_state_s4_table_clear(state);
    delete state->s4_table;
    state->s4_table = nullptr;

    instrumentr_state_char_table_clear(state);
    delete state->char_table;
    state->char_table = nullptr;

    instrumentr_state_symbol_table_clear(state);
    delete state->symbol_table;
    state->symbol_table = nullptr;

    instrumentr_state_character_table_clear(state);
    delete state->character_table;
    state->character_table = nullptr;

    instrumentr_state_integer_table_clear(state);
    delete state->integer_table;
    state->integer_table = nullptr;

    instrumentr_state_real_table_clear(state);
    delete state->real_table;
    state->real_table = nullptr;

    instrumentr_state_logical_table_clear(state);
    delete state->logical_table;
    state->logical_table = nullptr;

    instrumentr_state_complex_table_clear(state);
    delete state->complex_table;
    state->complex_table = nullptr;

    instrumentr_state_raw_table_clear(state);
    delete state->raw_table;
    state->raw_table = nullptr;

    SEXP r_result = PROTECT(instrumentr_state_as_list(state));

    instrumentr_state_clear(state);
    delete state->external;
    state->external = nullptr;

    UNPROTECT(1);

    return r_result;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DEFINE_API(state, INSTRUMENTR_OBJECT_TYPE_STATE)

/********************************************************************************
 * id
 *******************************************************************************/

int instrumentr_state_get_next_id(instrumentr_state_t state) {
    return (state->next_id)++;
}

/********************************************************************************
 * time
 *******************************************************************************/

int instrumentr_state_get_time(instrumentr_state_t state) {
    return state->time;
}

SEXP r_instrumentr_state_get_time(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    int time = instrumentr_state_get_time(state);
    return instrumentr_c_int_to_r_integer(time);
}

void instrumentr_state_increment_time(instrumentr_state_t state) {
    ++state->time;
}

/*******************************************************************************
 * alloc_stats
 *******************************************************************************/

instrumentr_alloc_stats_t
instrumentr_state_get_alloc_stats(instrumentr_state_t state) {
    return state->alloc_stats;
}

SEXP r_instrumentr_state_get_alloc_stats(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_alloc_stats_t stats = instrumentr_state_get_alloc_stats(state);
    return instrumentr_alloc_stats_as_data_frame(stats);
}

/*******************************************************************************
 * exec_stats
 *******************************************************************************/

instrumentr_exec_stats_t
instrumentr_state_get_exec_stats(instrumentr_state_t state) {
    return state->exec_stats;
}

SEXP r_instrumentr_state_get_exec_stats(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_exec_stats_t stats = instrumentr_state_get_exec_stats(state);
    return instrumentr_exec_stats_as_data_frame(stats);
}

/*******************************************************************************
 * empty
 *******************************************************************************/

/*  accessor  */
int instrumentr_state_is_empty(instrumentr_state_t state) {
    return state->external->empty();
}

SEXP r_instrumentr_state_is_empty(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    int result = instrumentr_state_is_empty(state);
    return instrumentr_c_int_to_r_logical(result);
}

/*******************************************************************************
 * size
 *******************************************************************************/

/*  accessor  */
int instrumentr_state_get_size(instrumentr_state_t state) {
    return state->external->size();
}

SEXP r_instrumentr_state_get_size(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    int result = instrumentr_state_get_size(state);
    return instrumentr_c_int_to_r_integer(result);
}

/*******************************************************************************
 * clear
 *******************************************************************************/

/*  mutator  */
void instrumentr_state_clear(instrumentr_state_t state) {
    for (auto iter = state->external->begin(); iter != state->external->end();
         ++iter) {
        instrumentr_sexp_release(iter->second);
    }

    state->external->clear();
}

SEXP r_instrumentr_state_clear(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_state_clear(state);
    return R_NilValue;
}

/*******************************************************************************
 * key
 *******************************************************************************/
/*  accessor  */
int instrumentr_state_has_key(instrumentr_state_t state, const char* key) {
    return state->external->find(std::string(key)) != state->external->end();
}

SEXP r_instrumentr_state_has_key(SEXP r_state, SEXP r_key) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* key = instrumentr_r_character_to_c_string(r_key);
    int result = instrumentr_state_has_key(state, key);
    return instrumentr_c_int_to_r_logical(result);
}

/*******************************************************************************
 * as_list
 *******************************************************************************/
/*  accessor  */
SEXP instrumentr_state_output_as_list(instrumentr_state_t state) {
    int size = instrumentr_state_get_size(state);

    SEXP r_keys = PROTECT(allocVector(STRSXP, size));
    SEXP r_values = PROTECT(allocVector(VECSXP, size));

    int index = 0;
    for (auto iter = state->external->begin(); iter != state->external->end();
         ++iter, ++index) {
        SET_STRING_ELT(r_keys, index, mkChar(iter->first.c_str()));
        SET_VECTOR_ELT(r_values, index, iter->second);
    }

    Rf_setAttrib(r_values, R_NamesSymbol, r_keys);
    UNPROTECT(2);

    return r_values;
}

/*  accessor  */
SEXP instrumentr_state_stats_as_list(instrumentr_state_t state) {
    SEXP r_keys = PROTECT(allocVector(STRSXP, 2));
    SEXP r_values = PROTECT(allocVector(VECSXP, 2));

    SET_STRING_ELT(r_keys, 0, mkChar("allocation"));
    SET_VECTOR_ELT(
        r_values, 0, instrumentr_alloc_stats_as_data_frame(state->alloc_stats));

    SET_STRING_ELT(r_keys, 1, mkChar("execution"));
    SET_VECTOR_ELT(
        r_values, 1, instrumentr_exec_stats_as_data_frame(state->exec_stats));

    Rf_setAttrib(r_values, R_NamesSymbol, r_keys);

    UNPROTECT(2);

    return r_values;
}

SEXP instrumentr_state_as_list(instrumentr_state_t state) {
    SEXP r_keys = PROTECT(allocVector(STRSXP, 2));
    SEXP r_values = PROTECT(allocVector(VECSXP, 2));

    SET_STRING_ELT(r_keys, 0, mkChar("output"));
    SET_VECTOR_ELT(r_values, 0, instrumentr_state_output_as_list(state));

    SET_STRING_ELT(r_keys, 1, mkChar("statistics"));
    SET_VECTOR_ELT(r_values, 1, instrumentr_state_stats_as_list(state));

    Rf_setAttrib(r_values, R_NamesSymbol, r_keys);
    UNPROTECT(2);

    return r_values;
}

SEXP r_instrumentr_state_as_list(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    return instrumentr_state_as_list(state);
}

/*******************************************************************************
 * lookup
 *******************************************************************************/
/*  accessor  */
SEXP instrumentr_state_lookup(instrumentr_state_t state,
                              const char* key,
                              SEXP r_alternative) {
    auto iter = state->external->find(std::string(key));
    if (iter == state->external->end()) {
        return r_alternative;
    } else {
        return iter->second;
    }
}

SEXP r_instrumentr_state_lookup(SEXP r_state, SEXP r_key, SEXP r_alternative) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* key = instrumentr_r_character_to_c_string(r_key);
    return instrumentr_state_lookup(state, key, r_alternative);
}

/*******************************************************************************
 * insert
 *******************************************************************************/
/*  mutator  */
void instrumentr_state_insert(instrumentr_state_t state,
                              const char* key,
                              SEXP r_value,
                              int overwrite) {
    auto iter = state->external->insert({std::string(key), r_value});
    /* insertion failed */
    if (!iter.second) {
        if (overwrite) {
            instrumentr_sexp_release(iter.first->second);
            iter.first->second = r_value;
            instrumentr_sexp_acquire(r_value);
        }
    }
    /* insertion succeeded  */
    else {
        instrumentr_sexp_acquire(r_value);
    }
}

SEXP r_instrumentr_state_insert(SEXP r_state,
                                SEXP r_key,
                                SEXP r_value,
                                SEXP r_overwrite) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* key = instrumentr_r_character_to_c_string(r_key);
    int overwrite = instrumentr_r_logical_to_c_int(r_overwrite);
    instrumentr_state_insert(state, key, r_value, overwrite);
    return R_NilValue;
}

/*******************************************************************************
 * erase
 *******************************************************************************/
/*  mutator  */
void instrumentr_state_erase(instrumentr_state_t state,
                             const char* key,
                             int permissive) {
    auto iter = state->external->find(std::string(key));

    /* not found  */
    if (iter == state->external->end()) {
        if (!permissive) {
            instrumentr_log_error("key '%s' not present in state for erasure",
                                  key);
        }
    }
    /* found  */
    else {
        instrumentr_sexp_release(iter->second);
        state->external->erase(iter);
    }
}

SEXP r_instrumentr_state_erase(SEXP r_state, SEXP r_key, SEXP r_permissive) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* key = instrumentr_r_character_to_c_string(r_key);
    int permissive = instrumentr_r_logical_to_c_int(r_permissive);
    instrumentr_state_erase(state, key, permissive);
    return R_NilValue;
}

/*******************************************************************************
 * call_stack
 *******************************************************************************/

/* accessor  */
instrumentr_call_stack_t
instrumentr_state_get_call_stack(instrumentr_state_t state) {
    return state->call_stack;
}

SEXP r_instrumentr_state_get_call_stack(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);
    return instrumentr_call_stack_wrap(call_stack);
}

/*******************************************************************************
 * promise_table
 *******************************************************************************/
instrumentr_promise_t
instrumentr_state_promise_table_create(instrumentr_state_t state,
                                       SEXP r_promise) {
    /* TODO: set promise birth time */
    instrumentr_promise_t promise =
        instrumentr_promise_create(state, r_promise);
    auto result = state->promise_table->insert({r_promise, promise});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = promise;
    }

    return promise;
}

void instrumentr_state_promise_table_remove(instrumentr_state_t state,
                                            SEXP r_promise) {
    auto result = state->promise_table->find(r_promise);
    if (result != state->promise_table->end()) {
        instrumentr_model_kill(result->second);
        state->promise_table->erase(result);
    }
}

instrumentr_promise_t
instrumentr_state_promise_table_lookup(instrumentr_state_t state,
                                       SEXP r_promise,
                                       int create) {
    auto result = state->promise_table->find(r_promise);
    if (result != state->promise_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_promise_table_create(state, r_promise);
    } else {
        instrumentr_log_error("promise %p not present in promise table",
                              r_promise);
        return NULL;
    }
}

void instrumentr_state_promise_table_clear(instrumentr_state_t state) {
    for (auto iter = state->promise_table->begin();
         iter != state->promise_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->promise_table->clear();
}

/*******************************************************************************
 * function_table
 *******************************************************************************/

instrumentr_function_t
instrumentr_state_function_table_create(instrumentr_state_t state,
                                        SEXP r_function) {
    instrumentr_function_t function =
        instrumentr_function_create(state, r_function);

    auto result = state->function_table->insert({r_function, function});

    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = function;
    }

    return function;
}

void instrumentr_state_function_table_remove(instrumentr_state_t state,
                                             SEXP r_function) {
    auto result = state->function_table->find(r_function);
    if (result != state->function_table->end()) {
        instrumentr_model_kill(result->second);
        state->function_table->erase(result);
    }
}

instrumentr_function_t
instrumentr_state_function_table_lookup(instrumentr_state_t state,
                                        SEXP r_function,
                                        int create) {
    auto result = state->function_table->find(r_function);
    if (result != state->function_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_function_table_create(state, r_function);
    } else {
        instrumentr_log_error("function %p not present in function table",
                              r_function);
        return NULL;
    }
}

void instrumentr_state_function_table_clear(instrumentr_state_t state) {
    for (auto iter = state->function_table->begin();
         iter != state->function_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->function_table->clear();
}

/*******************************************************************************
 * environment_table
 *******************************************************************************/

void instrumentr_state_environment_table_initialize(instrumentr_state_t state) {
    SEXP r_names = PROTECT(R_lsInternal(R_NamespaceRegistry, TRUE));

    for (int i = 0; i < Rf_length(r_names); ++i) {
        const char* name = CHAR(STRING_ELT(r_names, i));
        instrumentr_state_environment_table_update_namespace(state, name);
    }

    UNPROTECT(1);

    for (SEXP r_package = ENCLOS(R_GlobalEnv); r_package != R_EmptyEnv;
         r_package = ENCLOS(r_package)) {
        instrumentr_state_environment_table_lookup(state, r_package, 1);
    }
}

instrumentr_environment_t
instrumentr_state_environment_table_create(instrumentr_state_t state,
                                           SEXP r_environment) {
    /* TODO: set environment birth time */
    /* TODO: set names of package/namespace/environmentNames environments. */
    instrumentr_environment_t environment =
        instrumentr_environment_create(state, r_environment);
    auto result =
        state->environment_table->insert({r_environment, environment});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = environment;
    }

    return environment;
}

void instrumentr_state_environment_table_remove(instrumentr_state_t state,
                                                SEXP r_environment) {
    auto result = state->environment_table->find(r_environment);
    if (result != state->environment_table->end()) {
        instrumentr_model_kill(result->second);
        state->environment_table->erase(result);
    }
}

instrumentr_environment_t
instrumentr_state_environment_table_lookup(instrumentr_state_t state,
                                           SEXP r_environment,
                                           int create) {
    auto result = state->environment_table->find(r_environment);
    if (result != state->environment_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_environment_table_create(state, r_environment);
    } else {
        instrumentr_log_error("environment %p not present in environment table",
                              r_environment);
        return NULL;
    }
}

void instrumentr_state_environment_table_clear(instrumentr_state_t state) {
    for (auto iter = state->environment_table->begin();
         iter != state->environment_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->environment_table->clear();
}

/********************************************************************************
 * namespace
 *******************************************************************************/

int instrumentr_state_update_namespace_function_names(
    instrumentr_state_t state,
    SEXP r_namespace,
    instrumentr_environment_t environment) {
    SEXP r_names = PROTECT(R_lsInternal(r_namespace, TRUE));

    int counter = 0;

    for (int i = 0; i < Rf_length(r_names); ++i) {
        const char* name = CHAR(STRING_ELT(r_names, i));
        SEXP r_value = Rf_findVarInFrame(r_namespace, Rf_install(name));

        if (TYPEOF(r_value) == PROMSXP) {
            r_value = Rf_eval(r_value, r_namespace);
        }

        if (TYPEOF(r_value) != CLOSXP) {
            continue;
        }

        instrumentr_function_t function =
            instrumentr_state_function_table_lookup(state, r_value, true);

        SEXP r_value_env = CLOENV(r_value);

        /* only set function name if it the namespace under consideration
         * happens to be it's scope */
        if (r_value_env == r_namespace) {
            instrumentr_function_set_name(function, name);
        }
        ++counter;
        /* environment contains all function bindings */
        instrumentr_environment_insert(environment, name, function);
    }

    UNPROTECT(1);

    return counter;
}

void instrumentr_state_update_namespace_exports(
    instrumentr_state_t state,
    SEXP r_ns_inner,
    instrumentr_environment_t environment) {
    SEXP r_ns_exports = Rf_findVarInFrame(r_ns_inner, Rf_install("exports"));

    if (r_ns_exports == R_UnboundValue || TYPEOF(r_ns_exports) != ENVSXP) {
        return;
    }

    SEXP r_ns_exports_keys = R_lsInternal(r_ns_exports, TRUE);

    for (int i = 0; i < Rf_length(r_ns_exports_keys); ++i) {
        const char* key = CHAR(STRING_ELT(r_ns_exports_keys, i));

        /* TODO: value can be invalid (R_UnboundValue) */
        /* TODO: handle functions exported by pacakge A but defined by package
           B. graphics exports plot function defined in base package */
        if (instrumentr_environment_contains(environment, key)) {
            instrumentr_function_t function =
                instrumentr_environment_lookup(environment, key);

            instrumentr_function_set_exported(function);
        }
    }
}

void instrumentr_state_update_namespace_s3_methods(
    instrumentr_state_t state,
    SEXP r_ns_inner,
    instrumentr_environment_t environment) {
    SEXP r_s3_methods = Rf_findVarInFrame(r_ns_inner, Rf_install("S3methods"));

    if (r_s3_methods == R_UnboundValue || TYPEOF(r_s3_methods) != STRSXP) {
        return;
    }

    SEXP r_dims = Rf_getAttrib(r_s3_methods, R_DimNamesSymbol);

    if (r_dims != R_UnboundValue && TYPEOF(r_dims) == INTSXP &&
        Rf_length(r_dims) == 2) {
        int nrows = INTEGER(r_dims)[0];
        int ncols = INTEGER(r_dims)[0];

        if (ncols == 3) {
            for (int row_index = 0; row_index < nrows; ++row_index) {
                const char* generic_name =
                    CHAR(STRING_ELT(r_s3_methods, 0 * nrows + row_index));
                const char* object_class =
                    CHAR(STRING_ELT(r_s3_methods, 1 * nrows + row_index));
                const char* specific_name =
                    CHAR(STRING_ELT(r_s3_methods, 2 * nrows + row_index));

                /* TODO: value can be invalid (R_UnboundValue) */
                instrumentr_function_t function =
                    instrumentr_environment_lookup(environment, specific_name);

                instrumentr_function_set_object_class(function, object_class);
                instrumentr_function_set_generic_name(function, generic_name);
            }
        }
    }
}

instrumentr_environment_t
instrumentr_state_environment_table_update_namespace(instrumentr_state_t state,
                                                     const char* name) {
    SEXP r_namespace = Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(name));

    if (r_namespace == R_UnboundValue || TYPEOF(r_namespace) != ENVSXP) {
        instrumentr_log_error(
            "unable to find environment for namespace '%s' in registry", name);
    }

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_lookup(state, r_namespace, 1);

    instrumentr_environment_set_name(environment, name);

    SEXP r_ns_inner =
        Rf_findVarInFrame(r_namespace, Rf_install(".__NAMESPACE__."));

    if (r_ns_inner == R_UnboundValue) {
        instrumentr_log_message("cannot accesss inner namespace "
                                "'.__NAMESPACE__.' from namespace "
                                "environment %p for %\n",
                                r_namespace,
                                name);

        /* TODO: handle non inner ns cases */
        return environment;
    }

    int count = instrumentr_state_update_namespace_function_names(
        state, r_namespace, environment);
    instrumentr_state_update_namespace_exports(state, r_ns_inner, environment);
    instrumentr_state_update_namespace_s3_methods(
        state, r_ns_inner, environment);

    instrumentr_log_message("Added %d values to %s\n", count, name);

    return environment;
}

instrumentr_environment_t
instrumentr_state_environment_table_lookup_package(instrumentr_state_t state,
                                                   const char* package_name) {
    SEXP r_env = R_EmptyEnv;
    const char* name;

    for (r_env = R_GlobalEnv; r_env != R_EmptyEnv; r_env = ENCLOS(r_env)) {
        name = NULL;

        if (r_env == R_BaseEnv) {
            name = "base";
        } else if (R_IsPackageEnv(r_env)) {
            name = CHAR(STRING_ELT(R_PackageEnvName(r_env), 0)) +
                   strlen("package:");
        }

        if (name != NULL && !strcmp(name, package_name)) {
            break;
        }
    }

    if (r_env == R_EmptyEnv) {
        instrumentr_log_error("cannot find environment for package %s",
                              package_name);
    }

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_lookup(state, r_env, 1);

    instrumentr_environment_set_name(environment, name);

    return environment;
}

instrumentr_environment_t
instrumentr_state_environment_table_lookup_namespace(instrumentr_state_t state,
                                                     const char* package_name) {
    SEXP r_namespace =
        Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(package_name));

    if (r_namespace == R_UnboundValue || TYPEOF(r_namespace) != ENVSXP) {
        instrumentr_log_error("cannot find namespace for package %s",
                              package_name);
    }

    return instrumentr_state_environment_table_lookup(state, r_namespace, 1);
}

std::vector<instrumentr_environment_t>
instrumentr_state_get_packages(instrumentr_state_t state) {
    std::vector<instrumentr_environment_t> packages;

    for (SEXP r_package = ENCLOS(R_GlobalEnv); r_package != R_EmptyEnv;
         r_package = ENCLOS(r_package)) {
        if (R_IsPackageEnv(r_package)) {
            packages.push_back(instrumentr_state_environment_table_lookup(
                state, r_package, 0));
        }
    }
    return packages;
}

SEXP r_instrumentr_state_get_packages(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);

    std::vector<instrumentr_environment_t> packages =
        instrumentr_state_get_packages(state);

    SEXP r_packages = PROTECT(allocVector(VECSXP, packages.size()));
    SEXP r_names = PROTECT(allocVector(STRSXP, packages.size()));

    for (std::size_t i = 0; i < packages.size(); ++i) {
        instrumentr_environment_t environment = packages[i];
        const char* name = instrumentr_environment_get_name(environment);
        SET_VECTOR_ELT(
            r_packages, i, instrumentr_environment_wrap(environment));
        SET_STRING_ELT(r_names, i, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_packages, R_NamesSymbol, r_names);

    UNPROTECT(2);

    return r_packages;
}

std::vector<instrumentr_environment_t>
instrumentr_state_get_namespaces(instrumentr_state_t state) {
    SEXP r_names = R_lsInternal(R_NamespaceRegistry, TRUE);
    std::vector<instrumentr_environment_t> namespaces;

    for (int i = 0; i < Rf_length(r_names); ++i) {
        const char* name = CHAR(STRING_ELT(r_names, i));
        SEXP r_namespace =
            Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(name));
        namespaces.push_back(
            instrumentr_state_environment_table_lookup(state, r_namespace, 0));
    }
    return namespaces;
}

SEXP r_instrumentr_state_get_namespaces(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);

    std::vector<instrumentr_environment_t> namespaces =
        instrumentr_state_get_namespaces(state);

    SEXP r_namespaces = PROTECT(allocVector(VECSXP, namespaces.size()));
    SEXP r_names = PROTECT(allocVector(STRSXP, namespaces.size()));

    for (std::size_t i = 0; i < namespaces.size(); ++i) {
        instrumentr_environment_t environment = namespaces[i];
        const char* name = instrumentr_environment_get_name(environment);
        SET_VECTOR_ELT(
            r_namespaces, i, instrumentr_environment_wrap(environment));
        SET_STRING_ELT(r_names, i, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_namespaces, R_NamesSymbol, r_names);

    UNPROTECT(2);

    return r_namespaces;
}

instrumentr_miscellaneous_t
instrumentr_state_miscellaneous_table_create(instrumentr_state_t state,
                                             SEXP r_miscellaneous) {
    /* TODO: set miscellaneous birth time */
    /* TODO: set names of package/namespace/miscellaneousNames miscellaneouss.
     */
    instrumentr_miscellaneous_t miscellaneous =
        instrumentr_miscellaneous_create(state, r_miscellaneous);
    auto result =
        state->miscellaneous_table->insert({r_miscellaneous, miscellaneous});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = miscellaneous;
    }

    return miscellaneous;
}

void instrumentr_state_miscellaneous_table_remove(instrumentr_state_t state,
                                                  SEXP r_miscellaneous) {
    auto result = state->miscellaneous_table->find(r_miscellaneous);
    if (result != state->miscellaneous_table->end()) {
        instrumentr_model_kill(result->second);
        state->miscellaneous_table->erase(result);
    }
}

void instrumentr_state_miscellaneous_table_clear(instrumentr_state_t state) {
    for (auto iter = state->miscellaneous_table->begin();
         iter != state->miscellaneous_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->miscellaneous_table->clear();
}

/********************************************************************************
 * unbound_table
 *******************************************************************************/

instrumentr_unbound_t
instrumentr_state_unbound_table_create(instrumentr_state_t state,
                                       SEXP r_unbound) {
    /* TODO: set unbound birth time */
    /* TODO: set names of package/namespace/unboundNames unbounds.
     */
    instrumentr_unbound_t unboundval =
        instrumentr_unbound_create(state, r_unbound);
    auto result = state->unbound_table->insert({r_unbound, unboundval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = unboundval;
    }

    return unboundval;
}

void instrumentr_state_unbound_table_remove(instrumentr_state_t state,
                                            SEXP r_unbound) {
    auto result = state->unbound_table->find(r_unbound);
    if (result != state->unbound_table->end()) {
        instrumentr_model_kill(result->second);
        state->unbound_table->erase(result);
    }
}

instrumentr_unbound_t
instrumentr_state_unbound_table_lookup(instrumentr_state_t state,
                                       SEXP r_unbound,
                                       int create) {
    auto result = state->unbound_table->find(r_unbound);
    if (result != state->unbound_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_unbound_table_create(state, r_unbound);
    } else {
        instrumentr_log_error("unbound %p not present in unbound table",
                              r_unbound);
        return NULL;
    }
}

void instrumentr_state_unbound_table_clear(instrumentr_state_t state) {
    for (auto iter = state->unbound_table->begin();
         iter != state->unbound_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->unbound_table->clear();
}

/********************************************************************************
 * missing_table
 *******************************************************************************/

instrumentr_missing_t
instrumentr_state_missing_table_create(instrumentr_state_t state,
                                       SEXP r_missing) {
    /* TODO: set missing birth time */
    /* TODO: set names of package/namespace/missingNames missings.
     */
    instrumentr_missing_t missingval =
        instrumentr_missing_create(state, r_missing);
    auto result = state->missing_table->insert({r_missing, missingval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = missingval;
    }

    return missingval;
}

void instrumentr_state_missing_table_remove(instrumentr_state_t state,
                                            SEXP r_missing) {
    auto result = state->missing_table->find(r_missing);
    if (result != state->missing_table->end()) {
        instrumentr_model_kill(result->second);
        state->missing_table->erase(result);
    }
}

instrumentr_missing_t
instrumentr_state_missing_table_lookup(instrumentr_state_t state,
                                       SEXP r_missing,
                                       int create) {
    auto result = state->missing_table->find(r_missing);
    if (result != state->missing_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_missing_table_create(state, r_missing);
    } else {
        instrumentr_log_error("missing %p not present in missing table",
                              r_missing);
        return NULL;
    }
}

void instrumentr_state_missing_table_clear(instrumentr_state_t state) {
    for (auto iter = state->missing_table->begin();
         iter != state->missing_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->missing_table->clear();
}

/********************************************************************************
 * null_table
 *******************************************************************************/

instrumentr_null_t
instrumentr_state_null_table_create(instrumentr_state_t state, SEXP r_null) {
    /* TODO: set null birth time */
    /* TODO: set names of package/namespace/nullNames nulls.
     */
    instrumentr_null_t nullval = instrumentr_null_create(state, r_null);
    auto result = state->null_table->insert({r_null, nullval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = nullval;
    }

    return nullval;
}

void instrumentr_state_null_table_remove(instrumentr_state_t state,
                                         SEXP r_null) {
    auto result = state->null_table->find(r_null);
    if (result != state->null_table->end()) {
        instrumentr_model_kill(result->second);
        state->null_table->erase(result);
    }
}

instrumentr_null_t
instrumentr_state_null_table_lookup(instrumentr_state_t state,
                                    SEXP r_null,
                                    int create) {
    auto result = state->null_table->find(r_null);
    if (result != state->null_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_null_table_create(state, r_null);
    } else {
        instrumentr_log_error("null %p not present in null table", r_null);
        return NULL;
    }
}

void instrumentr_state_null_table_clear(instrumentr_state_t state) {
    for (auto iter = state->null_table->begin();
         iter != state->null_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->null_table->clear();
}

/********************************************************************************
 * externalptr_table
 *******************************************************************************/

instrumentr_externalptr_t
instrumentr_state_externalptr_table_create(instrumentr_state_t state,
                                           SEXP r_externalptr) {
    /* TODO: set externalptr birth time */
    /* TODO: set names of package/namespace/externalptrNames externalptrs.
     */
    instrumentr_externalptr_t externalptrval =
        instrumentr_externalptr_create(state, r_externalptr);
    auto result =
        state->externalptr_table->insert({r_externalptr, externalptrval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = externalptrval;
    }

    return externalptrval;
}

void instrumentr_state_externalptr_table_remove(instrumentr_state_t state,
                                                SEXP r_externalptr) {
    auto result = state->externalptr_table->find(r_externalptr);
    if (result != state->externalptr_table->end()) {
        instrumentr_model_kill(result->second);
        state->externalptr_table->erase(result);
    }
}

instrumentr_externalptr_t
instrumentr_state_externalptr_table_lookup(instrumentr_state_t state,
                                           SEXP r_externalptr,
                                           int create) {
    auto result = state->externalptr_table->find(r_externalptr);
    if (result != state->externalptr_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_externalptr_table_create(state, r_externalptr);
    } else {
        instrumentr_log_error("externalptr %p not present in externalptr table",
                              r_externalptr);
        return NULL;
    }
}

void instrumentr_state_externalptr_table_clear(instrumentr_state_t state) {
    for (auto iter = state->externalptr_table->begin();
         iter != state->externalptr_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->externalptr_table->clear();
}

/********************************************************************************
 * weakref_table
 *******************************************************************************/

instrumentr_weakref_t
instrumentr_state_weakref_table_create(instrumentr_state_t state,
                                       SEXP r_weakref) {
    /* TODO: set weakref birth time */
    /* TODO: set names of package/namespace/weakrefNames weakrefs.
     */
    instrumentr_weakref_t weakrefval =
        instrumentr_weakref_create(state, r_weakref);
    auto result = state->weakref_table->insert({r_weakref, weakrefval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = weakrefval;
    }

    return weakrefval;
}

void instrumentr_state_weakref_table_remove(instrumentr_state_t state,
                                            SEXP r_weakref) {
    auto result = state->weakref_table->find(r_weakref);
    if (result != state->weakref_table->end()) {
        instrumentr_model_kill(result->second);
        state->weakref_table->erase(result);
    }
}

instrumentr_weakref_t
instrumentr_state_weakref_table_lookup(instrumentr_state_t state,
                                       SEXP r_weakref,
                                       int create) {
    auto result = state->weakref_table->find(r_weakref);
    if (result != state->weakref_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_weakref_table_create(state, r_weakref);
    } else {
        instrumentr_log_error("weakref %p not present in weakref table",
                              r_weakref);
        return NULL;
    }
}

void instrumentr_state_weakref_table_clear(instrumentr_state_t state) {
    for (auto iter = state->weakref_table->begin();
         iter != state->weakref_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->weakref_table->clear();
}

/********************************************************************************
 * bytecode_table
 *******************************************************************************/

instrumentr_bytecode_t
instrumentr_state_bytecode_table_create(instrumentr_state_t state,
                                        SEXP r_bytecode) {
    /* TODO: set bytecode birth time */
    /* TODO: set names of package/namespace/bytecodeNames bytecodes.
     */
    instrumentr_bytecode_t bytecodeval =
        instrumentr_bytecode_create(state, r_bytecode);
    auto result = state->bytecode_table->insert({r_bytecode, bytecodeval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = bytecodeval;
    }

    return bytecodeval;
}

void instrumentr_state_bytecode_table_remove(instrumentr_state_t state,
                                             SEXP r_bytecode) {
    auto result = state->bytecode_table->find(r_bytecode);
    if (result != state->bytecode_table->end()) {
        instrumentr_model_kill(result->second);
        state->bytecode_table->erase(result);
    }
}

instrumentr_bytecode_t
instrumentr_state_bytecode_table_lookup(instrumentr_state_t state,
                                        SEXP r_bytecode,
                                        int create) {
    auto result = state->bytecode_table->find(r_bytecode);
    if (result != state->bytecode_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_bytecode_table_create(state, r_bytecode);
    } else {
        instrumentr_log_error("bytecode %p not present in bytecode table",
                              r_bytecode);
        return NULL;
    }
}

void instrumentr_state_bytecode_table_clear(instrumentr_state_t state) {
    for (auto iter = state->bytecode_table->begin();
         iter != state->bytecode_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->bytecode_table->clear();
}

/********************************************************************************
 * s4_table
 *******************************************************************************/

instrumentr_s4_t instrumentr_state_s4_table_create(instrumentr_state_t state,
                                                   SEXP r_s4) {
    /* TODO: set s4 birth time */
    /* TODO: set names of package/namespace/s4Names s4s.
     */
    instrumentr_s4_t s4val = instrumentr_s4_create(state, r_s4);
    auto result = state->s4_table->insert({r_s4, s4val});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = s4val;
    }

    return s4val;
}

void instrumentr_state_s4_table_remove(instrumentr_state_t state, SEXP r_s4) {
    auto result = state->s4_table->find(r_s4);
    if (result != state->s4_table->end()) {
        instrumentr_model_kill(result->second);
        state->s4_table->erase(result);
    }
}

instrumentr_s4_t instrumentr_state_s4_table_lookup(instrumentr_state_t state,
                                                   SEXP r_s4,
                                                   int create) {
    auto result = state->s4_table->find(r_s4);
    if (result != state->s4_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_s4_table_create(state, r_s4);
    } else {
        instrumentr_log_error("s4 %p not present in s4 table", r_s4);
        return NULL;
    }
}

void instrumentr_state_s4_table_clear(instrumentr_state_t state) {
    for (auto iter = state->s4_table->begin(); iter != state->s4_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->s4_table->clear();
}

/********************************************************************************
 * char_table
 *******************************************************************************/

instrumentr_char_t
instrumentr_state_char_table_create(instrumentr_state_t state, SEXP r_char) {
    /* TODO: set char birth time */
    /* TODO: set names of package/namespace/charNames chars.
     */
    instrumentr_char_t charval = instrumentr_char_create(state, r_char);
    auto result = state->char_table->insert({r_char, charval});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = charval;
    }

    return charval;
}

void instrumentr_state_char_table_remove(instrumentr_state_t state,
                                         SEXP r_char) {
    auto result = state->char_table->find(r_char);
    if (result != state->char_table->end()) {
        instrumentr_model_kill(result->second);
        state->char_table->erase(result);
    }
}

instrumentr_char_t
instrumentr_state_char_table_lookup(instrumentr_state_t state,
                                    SEXP r_char,
                                    int create) {
    auto result = state->char_table->find(r_char);
    if (result != state->char_table->end()) {
        return result->second;
    } else if (create) {
        return instrumentr_state_char_table_create(state, r_char);
    } else {
        instrumentr_log_error("char %p not present in char table", r_char);
        return NULL;
    }
}

void instrumentr_state_char_table_clear(instrumentr_state_t state) {
    for (auto iter = state->char_table->begin();
         iter != state->char_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->char_table->clear();
}

/********************************************************************************
 * symbol_table
 *******************************************************************************/

instrumentr_symbol_t
instrumentr_state_symbol_table_create(instrumentr_state_t state,
                                      SEXP r_symbol) {
    /* TODO: set symbol birth time */
    /* TODO: set names of package/namespace/symbolNames symbols.
     */
    instrumentr_symbol_t symbol = instrumentr_symbol_create(state, r_symbol);
    auto result = state->symbol_table->insert({r_symbol, symbol});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = symbol;
    }

    return symbol;
}

void instrumentr_state_symbol_table_remove(instrumentr_state_t state,
                                           SEXP r_symbol) {
    auto result = state->symbol_table->find(r_symbol);
    if (result != state->symbol_table->end()) {
        instrumentr_model_kill(result->second);
        state->symbol_table->erase(result);
    }
}

void instrumentr_state_symbol_table_clear(instrumentr_state_t state) {
    for (auto iter = state->symbol_table->begin();
         iter != state->symbol_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->symbol_table->clear();
}

/********************************************************************************
 * character_table
 *******************************************************************************/

instrumentr_character_t
instrumentr_state_character_table_create(instrumentr_state_t state,
                                         SEXP r_character) {
    /* TODO: set character birth time */
    /* TODO: set names of package/namespace/characterNames characters.
     */
    instrumentr_character_t character =
        instrumentr_character_create(state, r_character);
    auto result = state->character_table->insert({r_character, character});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = character;
    }

    return character;
}

void instrumentr_state_character_table_remove(instrumentr_state_t state,
                                              SEXP r_character) {
    auto result = state->character_table->find(r_character);
    if (result != state->character_table->end()) {
        instrumentr_model_kill(result->second);
        state->character_table->erase(result);
    }
}

void instrumentr_state_character_table_clear(instrumentr_state_t state) {
    for (auto iter = state->character_table->begin();
         iter != state->character_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->character_table->clear();
}

/********************************************************************************
 * integer_table
 *******************************************************************************/

instrumentr_integer_t
instrumentr_state_integer_table_create(instrumentr_state_t state,
                                       SEXP r_integer) {
    /* TODO: set integer birth time */
    /* TODO: set names of package/namespace/integerNames integers.
     */
    instrumentr_integer_t integer =
        instrumentr_integer_create(state, r_integer);
    auto result = state->integer_table->insert({r_integer, integer});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = integer;
    }

    return integer;
}

void instrumentr_state_integer_table_remove(instrumentr_state_t state,
                                            SEXP r_integer) {
    auto result = state->integer_table->find(r_integer);
    if (result != state->integer_table->end()) {
        instrumentr_model_kill(result->second);
        state->integer_table->erase(result);
    }
}

void instrumentr_state_integer_table_clear(instrumentr_state_t state) {
    for (auto iter = state->integer_table->begin();
         iter != state->integer_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->integer_table->clear();
}

/********************************************************************************
 * real_table
 *******************************************************************************/

instrumentr_real_t
instrumentr_state_real_table_create(instrumentr_state_t state, SEXP r_real) {
    /* TODO: set real birth time */
    /* TODO: set names of package/namespace/realNames reals.
     */
    instrumentr_real_t real = instrumentr_real_create(state, r_real);
    auto result = state->real_table->insert({r_real, real});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = real;
    }

    return real;
}

void instrumentr_state_real_table_remove(instrumentr_state_t state,
                                         SEXP r_real) {
    auto result = state->real_table->find(r_real);
    if (result != state->real_table->end()) {
        instrumentr_model_kill(result->second);
        state->real_table->erase(result);
    }
}

void instrumentr_state_real_table_clear(instrumentr_state_t state) {
    for (auto iter = state->real_table->begin();
         iter != state->real_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->real_table->clear();
}

/********************************************************************************
 * logical_table
 *******************************************************************************/

instrumentr_logical_t
instrumentr_state_logical_table_create(instrumentr_state_t state,
                                       SEXP r_logical) {
    /* TODO: set logical birth time */
    /* TODO: set names of package/namespace/logicalNames logicals.
     */
    instrumentr_logical_t logical =
        instrumentr_logical_create(state, r_logical);
    auto result = state->logical_table->insert({r_logical, logical});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = logical;
    }

    return logical;
}

void instrumentr_state_logical_table_remove(instrumentr_state_t state,
                                            SEXP r_logical) {
    auto result = state->logical_table->find(r_logical);
    if (result != state->logical_table->end()) {
        instrumentr_model_kill(result->second);
        state->logical_table->erase(result);
    }
}

void instrumentr_state_logical_table_clear(instrumentr_state_t state) {
    for (auto iter = state->logical_table->begin();
         iter != state->logical_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->logical_table->clear();
}

/********************************************************************************
 * complex_table
 *******************************************************************************/

instrumentr_complex_t
instrumentr_state_complex_table_create(instrumentr_state_t state,
                                       SEXP r_complex) {
    /* TODO: set complex birth time */
    /* TODO: set names of package/namespace/complexNames complexs.
     */
    instrumentr_complex_t complex =
        instrumentr_complex_create(state, r_complex);
    auto result = state->complex_table->insert({r_complex, complex});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = complex;
    }

    return complex;
}

void instrumentr_state_complex_table_remove(instrumentr_state_t state,
                                            SEXP r_complex) {
    auto result = state->complex_table->find(r_complex);
    if (result != state->complex_table->end()) {
        instrumentr_model_kill(result->second);
        state->complex_table->erase(result);
    }
}

void instrumentr_state_complex_table_clear(instrumentr_state_t state) {
    for (auto iter = state->complex_table->begin();
         iter != state->complex_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->complex_table->clear();
}

/********************************************************************************
 * raw_table
 *******************************************************************************/

instrumentr_raw_t instrumentr_state_raw_table_create(instrumentr_state_t state,
                                                     SEXP r_raw) {
    /* TODO: set raw birth time */
    /* TODO: set names of package/namespace/rawNames raws.
     */
    instrumentr_raw_t raw = instrumentr_raw_create(state, r_raw);
    auto result = state->raw_table->insert({r_raw, raw});
    if (!result.second) {
        /* TODO: this means the object was deallocated when tracing was
         * disabled. */
        instrumentr_model_kill(result.first->second);
        result.first->second = raw;
    }

    return raw;
}

void instrumentr_state_raw_table_remove(instrumentr_state_t state, SEXP r_raw) {
    auto result = state->raw_table->find(r_raw);
    if (result != state->raw_table->end()) {
        instrumentr_model_kill(result->second);
        state->raw_table->erase(result);
    }
}

void instrumentr_state_raw_table_clear(instrumentr_state_t state) {
    for (auto iter = state->raw_table->begin(); iter != state->raw_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->raw_table->clear();
}
