#include <instrumentr/Rincludes.h>
#include "object.h"
#include "state.h"
#include "interop.h"
#include <string>
#include <unordered_map>
#include "call_stack.h"
#include "frame.h"
#include "call.h"
#include "alloc_stats.h"
#include "exec_stats.h"
#include "value.h"
#include "values.h"

const int INSTRUMENTR_VALUE_TABLE_INITIAL_SIZE = 1000000;

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
    std::unordered_map<SEXP, instrumentr_value_t>* value_table;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_state_finalize(instrumentr_object_t object) {
    instrumentr_state_t state = (instrumentr_state_t) (object);

    if (state->external != nullptr) {
        instrumentr_state_clear(state);
        delete state->external;
        state->external = nullptr;
    }

    if (state->call_stack != nullptr) {
        instrumentr_call_stack_kill(state->call_stack);
        state->call_stack = nullptr;
    }

    if (state->value_table != nullptr) {
        instrumentr_state_value_table_clear(state);
        delete state->value_table;
        state->value_table = nullptr;
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

    state->value_table = new std::unordered_map<SEXP, instrumentr_value_t>();
    state->value_table->reserve(INSTRUMENTR_VALUE_TABLE_INITIAL_SIZE);

    state->call_stack = instrumentr_call_stack_create(state);

    return state;
}

/********************************************************************************
 * finalize tracing
 *******************************************************************************/

SEXP instrumentr_state_finalize_tracing(instrumentr_state_t state) {
    instrumentr_call_stack_kill(state->call_stack);
    state->call_stack = nullptr;

    instrumentr_state_value_table_clear(state);
    delete state->value_table;
    state->value_table = nullptr;

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
 * value_table
 *******************************************************************************/

void instrumentr_state_value_table_initialize(instrumentr_state_t state) {
    SEXP r_names = PROTECT(R_lsInternal(R_NamespaceRegistry, TRUE));

    for (int i = 0; i < Rf_length(r_names); ++i) {
        const char* name = CHAR(STRING_ELT(r_names, i));
        instrumentr_state_value_table_update_namespace(state, name);
    }

    UNPROTECT(1);

    for (SEXP r_package = ENCLOS(R_GlobalEnv); r_package != R_EmptyEnv;
         r_package = ENCLOS(r_package)) {
        instrumentr_state_value_table_insert(state, r_package);
    }
}

/*******************************************************************************
 * value
 ******************************************************************************/
bool instrumentr_value_is_consistent(instrumentr_value_t value) {
    SEXP r_sexp = instrumentr_value_get_sexp(value);

    if (r_sexp == R_UnboundValue) {
        return instrumentr_value_is_unbound(value);
    }

    else if (r_sexp == R_MissingArg) {
        return instrumentr_value_is_missing(value);
    }

    SEXPTYPE expected = TYPEOF(r_sexp);

    switch (expected) {
    case NILSXP:
        return instrumentr_value_is_null(value);
    case EXTPTRSXP:
        return instrumentr_value_is_externalptr(value);
    case WEAKREFSXP:
        return instrumentr_value_is_weakref(value);
    case BCODESXP:
        return instrumentr_value_is_bytecode(value);
    case S4SXP:
        return instrumentr_value_is_s4(value);
    case CHARSXP:
        return instrumentr_value_is_char(value);
    case SYMSXP:
        return instrumentr_value_is_symbol(value);
    case STRSXP:
        return instrumentr_value_is_character(value);
    case PROMSXP:
        return instrumentr_value_is_promise(value);
    case CLOSXP:
        return instrumentr_value_is_closure(value);
    case SPECIALSXP:
        return instrumentr_value_is_special(value);
    case BUILTINSXP:
        return instrumentr_value_is_builtin(value);
    case ENVSXP:
        return instrumentr_value_is_environment(value);
    case INTSXP:
        return instrumentr_value_is_integer(value);
    case REALSXP:
        return instrumentr_value_is_real(value);
    case LGLSXP:
        return instrumentr_value_is_logical(value);
    case CPLXSXP:
        return instrumentr_value_is_complex(value);
    case RAWSXP:
        return instrumentr_value_is_raw(value);
    case LANGSXP:
        return instrumentr_value_is_language(value);
    case DOTSXP:
        return instrumentr_value_is_dot(value);
    case LISTSXP:
        return instrumentr_value_is_pairlist(value);
    case VECSXP:
        return instrumentr_value_is_list(value);
    case EXPRSXP:
        return instrumentr_value_is_expression(value);
    default:
        return instrumentr_value_is_miscellaneous(value);
    }

    return false;
}

instrumentr_value_t instrumentr_state_create_value(instrumentr_state_t state,
                                                   SEXP r_object) {
    instrumentr_value_t value = NULL;

    if (r_object == R_UnboundValue) {
        value =
            (instrumentr_value_t) instrumentr_unbound_create(state, r_object);
    }

    else if (r_object == R_MissingArg) {
        value =
            (instrumentr_value_t) instrumentr_missing_create(state, r_object);
    }

    else if (TYPEOF(r_object) == NILSXP) {
        value = (instrumentr_value_t) instrumentr_null_create(state, r_object);
    }

    else if (TYPEOF(r_object) == EXTPTRSXP) {
        value = (instrumentr_value_t) instrumentr_externalptr_create(state,
                                                                     r_object);
    }

    else if (TYPEOF(r_object) == WEAKREFSXP) {
        value =
            (instrumentr_value_t) instrumentr_weakref_create(state, r_object);
    }

    else if (TYPEOF(r_object) == BCODESXP) {
        value =
            (instrumentr_value_t) instrumentr_bytecode_create(state, r_object);
    }

    else if (TYPEOF(r_object) == S4SXP) {
        value = (instrumentr_value_t) instrumentr_s4_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CHARSXP) {
        value = (instrumentr_value_t) instrumentr_char_create(state, r_object);
    }

    else if (TYPEOF(r_object) == SYMSXP) {
        value =
            (instrumentr_value_t) instrumentr_symbol_create(state, r_object);
    }

    else if (TYPEOF(r_object) == STRSXP) {
        value =
            (instrumentr_value_t) instrumentr_character_create(state, r_object);
    }

    else if (TYPEOF(r_object) == PROMSXP) {
        value =
            (instrumentr_value_t) instrumentr_promise_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CLOSXP) {
        value =
            (instrumentr_value_t) instrumentr_closure_create(state, r_object);
    }

    else if (TYPEOF(r_object) == SPECIALSXP) {
        value =
            (instrumentr_value_t) instrumentr_special_create(state, r_object);
    }

    else if (TYPEOF(r_object) == BUILTINSXP) {
        value =
            (instrumentr_value_t) instrumentr_builtin_create(state, r_object);
    }

    else if (TYPEOF(r_object) == ENVSXP) {
        value = (instrumentr_value_t) instrumentr_environment_create(state,
                                                                     r_object);
    }

    else if (TYPEOF(r_object) == INTSXP) {
        value =
            (instrumentr_value_t) instrumentr_integer_create(state, r_object);
    }

    else if (TYPEOF(r_object) == REALSXP) {
        value = (instrumentr_value_t) instrumentr_real_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LGLSXP) {
        value =
            (instrumentr_value_t) instrumentr_logical_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CPLXSXP) {
        value =
            (instrumentr_value_t) instrumentr_complex_create(state, r_object);
    }

    else if (TYPEOF(r_object) == RAWSXP) {
        value = (instrumentr_value_t) instrumentr_raw_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LANGSXP) {
        value =
            (instrumentr_value_t) instrumentr_language_create(state, r_object);
    }

    else if (TYPEOF(r_object) == DOTSXP) {
        value = (instrumentr_value_t) instrumentr_dot_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LISTSXP) {
        value =
            (instrumentr_value_t) instrumentr_pairlist_create(state, r_object);
    }

    else if (TYPEOF(r_object) == VECSXP) {
        value = (instrumentr_value_t) instrumentr_list_create(state, r_object);
    }

    else if (TYPEOF(r_object) == EXPRSXP) {
        value = (instrumentr_value_t) instrumentr_expression_create(state,
                                                                    r_object);
    }

    else {
        value = (instrumentr_value_t) instrumentr_miscellaneous_create(
            state, r_object);
    }

    return value;
}

instrumentr_value_t
instrumentr_state_value_table_insert(instrumentr_state_t state, SEXP r_object) {
    instrumentr_value_t value = instrumentr_state_create_value(state, r_object);
    /* TODO: set value birth time */
    auto result = state->value_table->insert({r_object, value});
    if (!result.second) {
        /* NOTE: this means the object was deallocated when tracing was
         * disabled. */
        auto iter = result.first;
        instrumentr_value_kill(iter->second);
        result.first->second = value;
    }

    return value;
}

void instrumentr_state_value_table_remove(instrumentr_state_t state,
                                          SEXP r_object) {
    auto result = state->value_table->find(r_object);
    if (result != state->value_table->end()) {
        instrumentr_value_kill(result->second);
        state->value_table->erase(result);
    }
}

std::vector<instrumentr_value_t>
instrumentr_state_value_table_get_values(instrumentr_state_t state) {
    std::vector<instrumentr_value_t> values;
    values.reserve(state->value_table->size());

    for (auto iter = state->value_table->begin();
         iter != state->value_table->end();
         ++iter) {
        instrumentr_value_t value = iter->second;
        if (instrumentr_value_is_closure(value) ||
            instrumentr_value_is_environment(value)) {
            values.push_back(value);
        }
    }

    return values;
}

instrumentr_value_t
instrumentr_state_value_table_lookup(instrumentr_state_t state,
                                     SEXP r_value,
                                     int create) {
    auto result = state->value_table->find(r_value);

    if (result != state->value_table->end()) {
        instrumentr_value_t value = result->second;

        if (!instrumentr_value_is_consistent(value)) {
            value = instrumentr_state_value_table_insert(
                state, instrumentr_value_get_sexp(value));
        }

        return value;

    } else if (create) {
        return instrumentr_state_value_table_insert(state, r_value);
    } else {
        instrumentr_log_error("value %p not present in value table", r_value);
        return NULL;
    }
}

instrumentr_value_t instrumentr_state_value_table_get(instrumentr_state_t state,
                                                      SEXP r_value) {
    auto result = state->value_table->find(r_value);

    if (result != state->value_table->end()) {
        return result->second;
    }
    return NULL;
}

instrumentr_closure_t
instrumentr_state_value_table_lookup_closure(instrumentr_state_t state,
                                             SEXP r_value,
                                             int create) {
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, create);
    return instrumentr_value_as_closure(value);
}

instrumentr_environment_t
instrumentr_state_value_table_lookup_environment(instrumentr_state_t state,
                                                 SEXP r_value,
                                                 int create) {
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, create);
    return instrumentr_value_as_environment(value);
}

instrumentr_promise_t
instrumentr_state_value_table_lookup_promise(instrumentr_state_t state,
                                             SEXP r_value,
                                             int create) {
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, create);
    return instrumentr_value_as_promise(value);
}

instrumentr_symbol_t
instrumentr_state_value_table_lookup_symbol(instrumentr_state_t state,
                                            SEXP r_value,
                                            int create) {
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, create);
    return instrumentr_value_as_symbol(value);
}

void instrumentr_state_value_table_clear(instrumentr_state_t state) {
    for (auto iter = state->value_table->begin();
         iter != state->value_table->end();
         ++iter) {
        instrumentr_value_kill(iter->second);
    }
    state->value_table->clear();
}

/********************************************************************************
 * namespace
 *******************************************************************************/

int instrumentr_state_update_namespace_closure_names(
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

        instrumentr_closure_t closure =
            instrumentr_state_value_table_lookup_closure(state, r_value, true);

        SEXP r_value_env = CLOENV(r_value);

        /* only set closure name if it the namespace under consideration
         * happens to be it's scope */
        if (r_value_env == r_namespace) {
            instrumentr_closure_set_name(closure, name);
        }
        ++counter;
        /* environment contains all closure bindings */
        instrumentr_environment_insert(environment, name, closure);
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

        instrumentr_symbol_t symbol =
            instrumentr_state_value_table_lookup_symbol(
                state, Rf_install(key), 1);

        instrumentr_value_t value =
            instrumentr_environment_lookup(environment, symbol);

        /* TODO: value can be invalid (R_UnboundValue) */
        /* TODO: handle closures exported by pacakge A but defined by package
           B. graphics exports plot closure defined in base package */
        if (instrumentr_value_is_closure(value)) {
            instrumentr_closure_t closure = instrumentr_value_as_closure(value);

            instrumentr_closure_set_exported(closure);
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

                instrumentr_symbol_t symbol =
                    instrumentr_state_value_table_lookup_symbol(
                        state, Rf_install(specific_name), 1);

                instrumentr_value_t value =
                    instrumentr_environment_lookup(environment, symbol);

                /* TODO: handle closures exported by pacakge A but defined by
                   package B. graphics exports plot closure defined in base
                   package */
                if (instrumentr_value_is_closure(value)) {
                    instrumentr_closure_t closure =
                        instrumentr_value_as_closure(value);

                    instrumentr_closure_set_object_class(closure, object_class);
                    instrumentr_closure_set_generic_name(closure, generic_name);
                }
            }
        }
    }
}

instrumentr_environment_t
instrumentr_state_value_table_update_namespace(instrumentr_state_t state,
                                               const char* name) {
    SEXP r_namespace = Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(name));

    if (r_namespace == R_UnboundValue || TYPEOF(r_namespace) != ENVSXP) {
        instrumentr_log_error(
            "unable to find environment for namespace '%s' in registry", name);
    }

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_environment(state, r_namespace, 1);

    instrumentr_environment_set_namespace(environment, name);

    int count = instrumentr_state_update_namespace_closure_names(
        state, r_namespace, environment);

    SEXP r_ns_inner =
        Rf_findVarInFrame(r_namespace, Rf_install(".__NAMESPACE__."));

    if (r_ns_inner != R_UnboundValue) {
        instrumentr_state_update_namespace_exports(
            state, r_ns_inner, environment);
        instrumentr_state_update_namespace_s3_methods(
            state, r_ns_inner, environment);

    } else {
        instrumentr_log_message("cannot accesss inner namespace "
                                "'.__NAMESPACE__.' from namespace "
                                "environment %p for %s\n",
                                r_namespace,
                                name);
    }

    instrumentr_log_message("Added %d values to %s\n", count, name);

    return environment;
}

instrumentr_environment_t
instrumentr_state_value_table_lookup_package(instrumentr_state_t state,
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
        instrumentr_state_value_table_lookup_environment(state, r_env, 1);

    instrumentr_environment_set_package(environment, name);

    return environment;
}

instrumentr_environment_t
instrumentr_state_value_table_lookup_namespace(instrumentr_state_t state,
                                               const char* package_name) {
    SEXP r_namespace =
        Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(package_name));

    if (r_namespace == R_UnboundValue || TYPEOF(r_namespace) != ENVSXP) {
        instrumentr_log_error("cannot find namespace for package %s",
                              package_name);
    }

    return instrumentr_state_value_table_lookup_environment(
        state, r_namespace, 1);
}

std::vector<instrumentr_environment_t>
instrumentr_state_get_packages(instrumentr_state_t state) {
    std::vector<instrumentr_environment_t> packages;

    for (SEXP r_package = ENCLOS(R_GlobalEnv); r_package != R_EmptyEnv;
         r_package = ENCLOS(r_package)) {
        if (R_IsPackageEnv(r_package)) {
            packages.push_back(instrumentr_state_value_table_lookup_environment(
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
        namespaces.push_back(instrumentr_state_value_table_lookup_environment(
            state, r_namespace, 0));
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

/*******************************************************************************
 symbol
 *******************************************************************************/

instrumentr_symbol_t instrumentr_state_get_symbol(instrumentr_state_t state,
                                                  const char* name) {
    SEXP r_name = Rf_install(name);
    return instrumentr_state_value_table_lookup_symbol(state, r_name, 1);
}

SEXP r_instrumentr_state_get_symbol(SEXP r_state, SEXP r_name) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_symbol_t symbol = instrumentr_state_get_symbol(state, name);
    return instrumentr_symbol_wrap(symbol);
}
