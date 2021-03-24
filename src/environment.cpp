#include "environment.h"
#include "interop.h"
#include "vec.h"
#include "utilities.h"
#include "funtab.h"
#include "closure.h"
#include "state.h"
#include "call.h"
#include "symbol.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_environment_impl_t {
    struct instrumentr_value_impl_t value;
    instrumentr_environment_type_t type;
    const char* name;
    std::unordered_map<std::string, instrumentr_closure_t>* bindings;
    instrumentr_call_t call;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_environment_finalize(instrumentr_value_t value) {
    instrumentr_environment_t environment = (instrumentr_environment_t)(value);

    free((char*) (environment->name));
    instrumentr_environment_clear(environment);
    delete environment->bindings;

    if (environment->type == INSTRUMENTR_ENVIRONMENT_TYPE_CALL) {
        instrumentr_call_release(environment->call);
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_environment_t
instrumentr_environment_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr environment */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_environment_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_ENVIRONMENT,
                                 instrumentr_environment_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_environment_t environment = (instrumentr_environment_t)(value);

    environment->type = INSTRUMENTR_ENVIRONMENT_TYPE_UNKNOWN;
    environment->name = NULL;

    environment->bindings =
        new std::unordered_map<std::string, instrumentr_closure_t>();

    environment->call = NULL;

    return environment;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_ENVIRONMENT,
                             environment,
                             environment)

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_environment_type_t
instrumentr_environment_get_type(instrumentr_environment_t environment) {
    return environment->type;
}

/* TODO: add R API */

void instrumentr_environment_set_type(instrumentr_environment_t environment,
                                      instrumentr_environment_type_t type) {
    environment->type = type;
}

std::string
instrumentr_environment_type_to_string(instrumentr_environment_type_t type) {
    switch (type) {
    case INSTRUMENTR_ENVIRONMENT_TYPE_UNKNOWN:
        return "unknown";
        break;

    case INSTRUMENTR_ENVIRONMENT_TYPE_NAMESPACE:
        return "namespace";
        break;

    case INSTRUMENTR_ENVIRONMENT_TYPE_PACKAGE:
        return "package";
        break;

    case INSTRUMENTR_ENVIRONMENT_TYPE_CALL:
        return "call";
        break;

    case INSTRUMENTR_ENVIRONMENT_TYPE_COUNT:
        instrumentr_log_error(
            "environment type 'INSTRUMENTR_ENVIRONMENT_TYPE_COUNT' is reserved "
            "for internal use only");
    }
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_environment_get_name(instrumentr_environment_t environment) {
    return environment->name;
}

SEXP r_instrumentr_environment_get_name(SEXP r_environment) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);
    const char* name = instrumentr_environment_get_name(environment);
    return instrumentr_c_string_to_r_character(name);
}

/* mutator */
void instrumentr_environment_set_name(instrumentr_environment_t environment,
                                      const char* name) {
    environment->name = instrumentr_duplicate_string(name);
}

/********************************************************************************
 * closures
 *******************************************************************************/

/* accessor  */
int instrumentr_environment_get_size(instrumentr_environment_t environment) {
    return environment->bindings->size();
}

SEXP r_instrumentr_environment_get_size(SEXP r_environment) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);
    int size = instrumentr_environment_get_size(environment);
    return instrumentr_c_int_to_r_integer(size);
}

/* accessor  */
instrumentr_value_t
instrumentr_environment_lookup(instrumentr_environment_t environment,
                               instrumentr_symbol_t symbol) {
    SEXP r_name = instrumentr_symbol_get_sexp(symbol);

    SEXP r_env = instrumentr_environment_get_sexp(environment);

    SEXP r_result = Rf_findVarInFrame(r_env, r_name);

    instrumentr_state_t state = instrumentr_environment_get_state(environment);

    return instrumentr_state_value_table_lookup(state, r_result, 1);
}

SEXP r_instrumentr_environment_lookup(SEXP r_environment, SEXP r_symbol) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);

    instrumentr_symbol_t symbol = instrumentr_symbol_unwrap(r_symbol);

    instrumentr_value_t value =
        instrumentr_environment_lookup(environment, symbol);

    return instrumentr_value_wrap(value);
}

/* accessor  */
int instrumentr_environment_contains(instrumentr_environment_t environment,
                                     const char* name) {
    auto iter = environment->bindings->find(name);

    return iter != environment->bindings->end();
}

SEXP r_instrumentr_environment_contains(SEXP r_environment, SEXP r_name) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);

    const char* name = CHAR(STRING_ELT(r_name, 0));

    int result = instrumentr_environment_contains(environment, name);

    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
void instrumentr_environment_insert(instrumentr_environment_t environment,
                                    const char* name,
                                    instrumentr_closure_t closure) {
    auto result = environment->bindings->insert({name, closure});

    if (!result.second) {
        instrumentr_log_error(
            "binding with name %s already exists in environment %p",
            name,
            instrumentr_environment_get_sexp(environment));
    }

    instrumentr_closure_acquire(closure);
}

/* accessor  */
const std::unordered_map<std::string, instrumentr_closure_t>&
instrumentr_environment_get_bindings(instrumentr_environment_t environment) {
    return *(environment->bindings);
}

/* accessor  */
SEXP r_instrumentr_environment_get_bindings(SEXP r_environment) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);

    int size = instrumentr_environment_get_size(environment);

    SEXP r_closures = PROTECT(allocVector(VECSXP, size));
    SEXP r_names = PROTECT(allocVector(STRSXP, size));

    int index = 0;

    for (auto iter = environment->bindings->begin();
         iter != environment->bindings->end();
         ++iter, ++index) {
        const std::string& name = iter->first;
        instrumentr_closure_t closure = iter->second;

        SET_VECTOR_ELT(r_closures, index, instrumentr_closure_wrap(closure));
        SET_STRING_ELT(r_names, index, mkChar(name.c_str()));
    }

    Rf_setAttrib(r_closures, R_NamesSymbol, r_names);

    UNPROTECT(2);
    return r_closures;
}

/* mutator */
void instrumentr_environment_clear(instrumentr_environment_t environment) {
    for (auto iter = environment->bindings->begin();
         iter != environment->bindings->end();
         ++iter) {
        instrumentr_closure_t closure = iter->second;
        instrumentr_closure_release(closure);
    }

    environment->bindings->clear();
}

void instrumentr_environment_set_call(instrumentr_environment_t environment,
                                      instrumentr_call_t call) {
    environment->type = INSTRUMENTR_ENVIRONMENT_TYPE_CALL;
    environment->call = call;
    instrumentr_call_acquire(environment->call);
}

instrumentr_call_t
instrumentr_environment_get_call(instrumentr_environment_t environment) {
    if (environment->type != INSTRUMENTR_ENVIRONMENT_TYPE_CALL) {
        instrumentr_log_error(
            "cannot get call for an '%s' environment",
            instrumentr_environment_type_to_string(environment->type).c_str());
    }
    return environment->call;
}

void instrumentr_environment_set_package(instrumentr_environment_t environment,
                                         const char* name) {
    environment->type = INSTRUMENTR_ENVIRONMENT_TYPE_PACKAGE;
    instrumentr_environment_set_name(environment, name);
}

void instrumentr_environment_set_namespace(
    instrumentr_environment_t environment,
    const char* name) {
    environment->type = INSTRUMENTR_ENVIRONMENT_TYPE_NAMESPACE;
    instrumentr_environment_set_name(environment, name);
}
