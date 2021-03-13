#include <instrumentr/Rincludes.h>
#include "object.h"
#include "state.h"
#include "interop.h"
#include <string>
#include <unordered_map>
#include "promise.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_state_impl_t {
    struct instrumentr_object_impl_t object;
    int next_id;
    int time;
    std::unordered_map<std::string, SEXP>* external;
    std::unordered_map<SEXP, instrumentr_promise_t>* promise_table;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_state_finalize(instrumentr_object_t object) {
    instrumentr_state_t state = (instrumentr_state_t)(object);
    instrumentr_state_clear(state);
    delete state->external;
    state->external = nullptr;

    instrumentr_state_promise_table_clear(state);
    delete state->promise_table;
    state->promise_table = nullptr;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_state_t instrumentr_state_create() {
    instrumentr_state_t state = (instrumentr_state_t) instrumentr_object_create(
        sizeof(instrumentr_state_impl_t));

    state->next_id = 0;
    state->time = -1;
    state->external = new std::unordered_map<std::string, SEXP>();
    state->promise_table =
        new std::unordered_map<SEXP, instrumentr_promise_t>();

    instrumentr_object_initialize((instrumentr_object_t) state,
                                  state,
                                  INSTRUMENTR_STATE,
                                  instrumentr_state_finalize,
                                  INSTRUMENTR_ORIGIN_FOREIGN);
    return state;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_state_wrap(instrumentr_state_t state) {
    return instrumentr_object_wrap((instrumentr_object_t)(state));
}

instrumentr_state_t instrumentr_state_unwrap(SEXP r_state) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_state, INSTRUMENTR_STATE);
    return (instrumentr_state_t)(object);
}

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

void instrumentr_state_increment_time(instrumentr_state_t state) {
    ++state->time;
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
SEXP instrumentr_state_as_list(instrumentr_state_t state) {
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
 * promise_table
 *******************************************************************************/
instrumentr_promise_t
instrumentr_state_promise_table_create(instrumentr_state_t state,
                                       SEXP r_promise) {
    /* TODO: set promise birth time */
    instrumentr_promise_t promise = instrumentr_promise_create(state, r_promise);
    auto result = state->promise_table->insert({r_promise, promise});
    if (!result.second) {
        instrumentr_object_release(result.first->second);
        result.first->second = promise;
    }

    return promise;
}

void instrumentr_state_promise_table_remove(instrumentr_state_t state,
                                            SEXP r_promise) {
    auto result = state->promise_table->find(r_promise);
    if (result != state->promise_table->end()) {
        instrumentr_object_release(result->second);
        state->promise_table->erase(result);
        /* TODO: kill promise */
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
        instrumentr_object_release(iter->second);
    }
    state->promise_table->clear();
}
