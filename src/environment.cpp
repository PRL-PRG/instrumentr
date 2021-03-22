#include "environment.h"
#include "interop.h"
#include "vec.h"
#include "utilities.h"
#include "funtab.h"
#include "closure.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_environment_impl_t {
    struct instrumentr_model_impl_t model;
    instrumentr_environment_type_t type;
    SEXP r_sexp;
    const char* name;
    std::unordered_map<std::string, instrumentr_closure_t>* bindings;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_environment_finalize(instrumentr_model_t model) {
    instrumentr_environment_t environment = (instrumentr_environment_t)(model);

    environment->r_sexp = NULL;
    free((char*) (environment->name));
    instrumentr_environment_clear(environment);
    delete environment->bindings;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_environment_t
instrumentr_environment_create(instrumentr_state_t state, SEXP r_sexp) {
    /* TODO: make foreign for instrumentr environment */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_environment_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_ENVIRONMENT,
                                 instrumentr_environment_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_environment_t environment = (instrumentr_environment_t)(model);

    environment->type = INSTRUMENTR_ENVIRONMENT_TYPE_UNKNOWN;
    environment->name = NULL;

    environment->r_sexp = r_sexp;

    environment->bindings =
        new std::unordered_map<std::string, instrumentr_closure_t>();

    // for base env
    // if (strcmp(name, "base") == 0) {
    //    int funtab_size = instrumentr_funtab_get_size();
    //    for (int i = 0; i < funtab_size; ++i) {
    //        vec_push(&environment->basic_closures,
    //                 instrumentr_funtab_create_closure(state, i));
    //    }
    //}

    return environment;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(environment,
                                     INSTRUMENTR_MODEL_TYPE_ENVIRONMENT)

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
 * r_namespace
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_environment_get_sexp(instrumentr_environment_t environment) {
    return environment->r_sexp;
}

SEXP r_instrumentr_environment_get_sexp(SEXP r_environment) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);
    return instrumentr_environment_get_sexp(environment);
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
instrumentr_closure_t
instrumentr_environment_lookup(instrumentr_environment_t environment,
                               const char* name) {
    auto iter = environment->bindings->find(name);

    if (iter == environment->bindings->end()) {
        instrumentr_log_error("cannot find closure %s in environment %p",
                              name,
                              environment->r_sexp);
    }

    return iter->second;
}

SEXP r_instrumentr_environment_lookup(SEXP r_environment, SEXP r_name) {
    instrumentr_environment_t environment =
        instrumentr_environment_unwrap(r_environment);

    const char* name = CHAR(STRING_ELT(r_name, 0));

    instrumentr_closure_t closure =
        instrumentr_environment_lookup(environment, name);

    return instrumentr_closure_wrap(closure);
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
            environment->r_sexp);
    }

    instrumentr_model_acquire(closure);
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
        instrumentr_model_release(closure);
    }

    environment->bindings->clear();
}
