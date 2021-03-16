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
#include "package.h"
#include "alloc_stats.h"
#include "exec_stats.h"

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
    std::unordered_map<std::string, instrumentr_package_t>* package_table;
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

    if (state->package_table != nullptr) {
        instrumentr_state_package_table_clear(state);
        delete state->package_table;
        state->package_table = nullptr;
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

    state->function_table =
        new std::unordered_map<SEXP, instrumentr_function_t>();

    state->package_table =
        new std::unordered_map<std::string, instrumentr_package_t>();

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

    instrumentr_state_package_table_clear(state);
    delete state->package_table;
    state->package_table = nullptr;

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
        // fprintf(stderr, "ref count on lookup: %d\n",
        //        instrumentr_object_get_ref_count(result->second));
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
instrumentr_state_function_table_lookup(instrumentr_state_t state,
                                        SEXP r_closure,
                                        SEXP r_call) {
    instrumentr_function_t function = NULL;

    auto result = state->function_table->find(r_closure);

    /* the function is already present */
    if (result != state->function_table->end()) {
        function = result->second;
    }
    /* create the function and add to cache if function is not present */
    else {
        function = instrumentr_state_function_table_add(state, r_closure);
    }

    /* the function does not have a name */
    if (!instrumentr_function_has_name(function)) {
        SEXP r_call_name = CAR(r_call);

        /* the call has a name which could be function's canonical name */
        if (TYPEOF(r_call_name) == SYMSXP) {
            SEXP r_lexenv = CLOENV(r_closure);
            SEXP r_result = Rf_findVarInFrame(r_lexenv, r_call_name);

            int is_bound = r_result == r_closure;
            is_bound =
                is_bound ||
                TYPEOF(r_result) == PROMSXP &&
                    (dyntrace_get_promise_value(r_result) == r_closure ||
                     dyntrace_get_promise_expression(r_result) == r_closure);

            if (is_bound) {
                instrumentr_function_set_name(function,
                                              CHAR(PRINTNAME(r_call_name)));
            }
        }
    }

    return function;
}

instrumentr_function_t
instrumentr_state_function_table_insert(instrumentr_state_t state,
                                        instrumentr_function_t function,
                                        SEXP r_closure) {
    auto result = state->function_table->insert({r_closure, function});

    bool inserted = result.second;

    if (!inserted) {
        /* TODO: remove this from package as well */
        instrumentr_model_kill(result.first->second);
        result.first->second = function;
        // TODO: uncommenting this causes errors. fix them!
        // instrumentr_log_error(
        //    "unable to insert a new function object to function map,
        //    perhaps " "the cache has not been cleaned of old objects?");
    }

    instrumentr_model_acquire(function);

    return function;
}

void instrumentr_state_function_table_remove(instrumentr_state_t state,
                                             SEXP r_closure) {
    auto result = state->function_table->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != state->function_table->end()) {
        instrumentr_model_release(result->second);
        state->function_table->erase(r_closure);
    }
}

instrumentr_function_t
instrumentr_state_function_table_add(instrumentr_state_t state,
                                     SEXP r_closure) {
    int alien = 0;
    instrumentr_function_t parent = NULL;
    instrumentr_function_t function = NULL;
    const char* function_name = NULL;

    SEXP r_lexenv = CLOENV(r_closure);

    SEXP r_env_name = R_PackageEnvName(r_lexenv);

    if (r_env_name != R_NilValue) {
    }

    /* if the function is not a package function */
    if (r_env_name == R_NilValue) {
        instrumentr_call_stack_t call_stack =
            instrumentr_state_get_call_stack(state);
        int size = instrumentr_call_stack_get_size(call_stack);

        for (int i = size - 1; i >= 0; --i) {
            instrumentr_frame_t frame =
                instrumentr_call_stack_get_frame(call_stack, i);
            if (!instrumentr_frame_is_call(frame)) {
                continue;
            }
            instrumentr_call_t call = instrumentr_frame_as_call(frame);
            instrumentr_function_t call_fun =
                instrumentr_call_get_function(call);

            /* call's environment is function's lexical environment */
            if (instrumentr_function_is_closure(call_fun) &&
                instrumentr_call_get_environment(call) == r_lexenv) {
                parent = call_fun;
                break;
            }
        }
    }
    /* TODO: memory management - function will not be collected if insert
     * fails. */
    function = instrumentr_function_create_closure(state,
                                                   function_name,
                                                   Rf_length(CAR(r_closure)),
                                                   r_closure,
                                                   parent,
                                                   0,
                                                   0,
                                                   0
                                                   /*,
                                                   alien,
                                                   parent*/);

    instrumentr_state_function_table_insert(state, function, r_closure);

    instrumentr_model_release(function);

    return function;
}

void instrumentr_state_function_table_update_name(instrumentr_state_t state,
                                                  SEXP r_symbol,
                                                  SEXP r_value,
                                                  SEXP r_rho) {
    SEXP r_closure = NULL;

    if (TYPEOF(r_value) == CLOSXP) {
        r_closure = r_value;
    } else if (TYPEOF(r_value) == PROMSXP) {
        SEXP r_promval = dyntrace_get_promise_value(r_value);

        if (TYPEOF(r_promval) == CLOSXP) {
            r_closure = r_promval;
        } else {
            SEXP r_promexpr = dyntrace_get_promise_expression(r_value);
            if (TYPEOF(r_promexpr) == CLOSXP) {
                r_closure = r_promexpr;
            }
        }
    }

    if (r_closure == NULL) {
        return;
    }

    SEXP r_lexenv = CLOENV(r_closure);

    bool in_scope = r_lexenv == r_rho;

    const char* name = in_scope ? CHAR(PRINTNAME(r_symbol)) : NULL;

    instrumentr_function_t function = NULL;

    auto result = state->function_table->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != state->function_table->end()) {
        function = result->second;
        if (!instrumentr_function_has_name(function)) {
            instrumentr_function_set_name(function, name);
        }
    }
    /* this means the function is alien */
    else {
        int alien = 1;
        instrumentr_function_t parent = NULL;

        /* TODO: memory management - function will not be collected if
         * insert fails. */
        function =
            instrumentr_function_create_closure(state,
                                                name,
                                                Rf_length(CAR(r_closure)),
                                                r_closure,
                                                parent,
                                                0,
                                                0,
                                                0 /*, alien, parent*/);

        instrumentr_state_function_table_insert(state, function, r_closure);

        instrumentr_model_release(function);
    }
}

/* NOTE: this function is called from package load hook for top level
 * package functions. It updates function properties and adds them to
 * package */
SEXP r_instrumentr_state_function_table_update_properties(SEXP r_state,
                                                          SEXP r_package,
                                                          SEXP r_name,
                                                          SEXP r_closure,
                                                          SEXP r_rho,
                                                          SEXP r_pub,
                                                          SEXP r_s3_generic,
                                                          SEXP r_s3_method) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);

    instrumentr_function_t parent = NULL;
    int pub = asLogical(r_pub);
    int s3_generic = asLogical(r_s3_generic);
    int s3_method = asLogical(r_s3_method);

    const char* name = CHAR(STRING_ELT(r_name, 0));

    instrumentr_function_t function = NULL;

    auto result = state->function_table->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != state->function_table->end()) {
        function = result->second;
        instrumentr_function_set_name(function, name);
        instrumentr_function_set_public(function, pub);
        instrumentr_function_set_s3_generic(function, s3_generic);
        instrumentr_function_set_s3_method(function, s3_method);
    }
    /* this means the function is alien */
    else {
        /* TODO: memory management - function will not be collected if
         * insert fails. */
        function =
            instrumentr_function_create_closure(state,
                                                name,
                                                Rf_length(CAR(r_closure)),
                                                r_closure,
                                                parent,
                                                pub,
                                                s3_generic,
                                                s3_method /*, alien, */);

        instrumentr_state_function_table_insert(state, function, r_closure);

        instrumentr_model_release(function);
    }

    instrumentr_package_t package = instrumentr_package_unwrap(r_package);

    if (!instrumentr_package_has_function(package, name)) {
        instrumentr_package_add_function(package, function);
    }

    return R_NilValue;
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
 * package_table
 *******************************************************************************/

/* accessor  */
int instrumentr_state_get_package_count(instrumentr_state_t state) {
    return state->package_table->size();
}

SEXP r_instrumentr_state_get_package_count(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    int count = instrumentr_state_get_package_count(state);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_package_t instrumentr_state_get_package(instrumentr_state_t state,
                                                    const char* name) {
    auto iter = state->package_table->find(std::string(name));

    if (iter == state->package_table->end()) {
        instrumentr_log_error("package '%s' does not exist", name);
    }

    return iter->second;
}

SEXP r_instrumentr_state_get_package(SEXP r_state, SEXP r_name) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_package_t package = instrumentr_state_get_package(state, name);
    return instrumentr_package_wrap(package);
}

/* accessor */
instrumentr_package_t
instrumentr_state_get_base_package(instrumentr_state_t state) {
    return instrumentr_state_get_package(state, "base");
}

SEXP r_instrumentr_state_get_base_package(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_package_t package = instrumentr_state_get_base_package(state);
    return instrumentr_package_wrap(package);
}

/* accessor */
SEXP r_instrumentr_state_get_packages(SEXP r_state) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);

    int count = instrumentr_state_get_package_count(state);

    SEXP r_packages = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    int index = 0;

    for (auto iter = state->package_table->begin();
         iter != state->package_table->end();
         ++iter, ++index) {
        instrumentr_package_t package = iter->second;
        const char* name = instrumentr_package_get_name(package);
        SET_VECTOR_ELT(r_packages, index, instrumentr_package_wrap(package));
        SET_STRING_ELT(r_names, index, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_packages, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_packages;
}

/* mutator */
void instrumentr_state_add_package(instrumentr_state_t state,
                                   instrumentr_package_t package) {
    const char* name = instrumentr_package_get_name(package);

    auto result = state->package_table->insert({name, package});

    if (!result.second) {
        instrumentr_log_error("package '%s' already exists", name);
    }

    instrumentr_model_acquire(package);
}

/* mutator */
SEXP r_instrumentr_state_add_package(SEXP r_state, SEXP r_package) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_state_add_package(state, package);
    return R_NilValue;
}

/* mutator */
void instrumentr_state_remove_package(instrumentr_state_t state,
                                      instrumentr_package_t package) {
    const char* name = instrumentr_package_get_name(package);

    auto iter = state->package_table->find(name);

    if (iter == state->package_table->end()) {
        instrumentr_log_error("package '%s' cannot be removed", name);
    }

    state->package_table->erase(iter);

    instrumentr_model_kill(package);
}

void instrumentr_state_package_table_clear(instrumentr_state_t state) {
    for (auto iter = state->package_table->begin();
         iter != state->package_table->end();
         ++iter) {
        instrumentr_model_kill(iter->second);
    }
    state->package_table->clear();
}
