#include "object.h"
#include "application.h"
#include "package.h"
#include "function.h"
#include "call_stack.h"
#include "interop.h"
#include "utilities.h"
#include "vec.h"
#include <unordered_map>
#include "frame.h"
#include "state.h"
#include "call.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_package_t) instrumentr_package_vector_t;

struct instrumentr_application_impl_t {
    struct instrumentr_object_impl_t object;
    const char* name;
    const char* directory;
    SEXP r_code;
    SEXP r_environment;
    int frame_position;
    instrumentr_call_stack_t call_stack;
    instrumentr_package_vector_t packages;
    std::unordered_map<SEXP, instrumentr_function_t>* function_map;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_application_finalize(instrumentr_object_t object) {
    instrumentr_application_t application = (instrumentr_application_t)(object);

    free((char*) (application->name));

    free((char*) (application->directory));

    instrumentr_sexp_release(application->r_code);
    application->r_code = NULL;

    instrumentr_sexp_release(application->r_environment);
    application->r_environment = NULL;

    application->frame_position = 0;

    instrumentr_object_release(application->call_stack);
    application->call_stack = NULL;

    int count = application->packages.length;
    instrumentr_package_t* packages = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        instrumentr_object_release(package);
    }

    vec_deinit(&application->packages);

    delete application->function_map;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_application_t
instrumentr_application_create(instrumentr_state_t state,
                               const char* const name,
                               const char* const directory,
                               SEXP r_code,
                               SEXP r_environment,
                               int frame_position) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);

    instrumentr_object_t object = instrumentr_object_create_and_initialize(
        sizeof(struct instrumentr_application_impl_t),
        state,
        INSTRUMENTR_APPLICATION,
        instrumentr_application_finalize,
        INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_application_t application = (instrumentr_application_t)(object);

    application->name = duplicate_name;
    application->directory = duplicate_directory;

    application->r_code = r_code;
    instrumentr_sexp_acquire(application->r_code);

    application->r_environment = r_environment;
    instrumentr_sexp_acquire(application->r_environment);

    application->frame_position = frame_position;

    application->call_stack = instrumentr_call_stack_create(state);

    vec_init(&application->packages);

    application->function_map =
        new std::unordered_map<SEXP, instrumentr_function_t>();

    return application;
}

SEXP r_instrumentr_application_create(SEXP r_state,
                                      SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* const name = instrumentr_r_character_to_c_string(r_name);
    const char* const directory =
        instrumentr_r_character_to_c_string(r_directory);
    int frame_position = instrumentr_r_integer_to_c_int(r_frame_position);
    instrumentr_application_t application = instrumentr_application_create(
        state, name, directory, r_code, r_environment, frame_position);
    SEXP r_application = instrumentr_application_wrap(application);
    instrumentr_object_release(application);
    return r_application;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_application_wrap(instrumentr_application_t application) {
    return instrumentr_object_wrap((instrumentr_object_t)(application));
}

instrumentr_application_t instrumentr_application_unwrap(SEXP r_application) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_application, INSTRUMENTR_APPLICATION);
    return (instrumentr_application_t)(object);
}

/*******************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_application_get_name(instrumentr_application_t application) {
    return application->name;
}

SEXP r_instrumentr_application_get_name(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* name = instrumentr_application_get_name(application);
    return instrumentr_c_string_to_r_character(name);
}

/*******************************************************************************
 * directory
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_application_get_directory(instrumentr_application_t application) {
    return application->directory;
}

SEXP r_instrumentr_application_get_directory(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* directory = instrumentr_application_get_directory(application);
    return instrumentr_c_string_to_r_character(directory);
}

/*******************************************************************************
 * r_code
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_application_get_code(instrumentr_application_t application) {
    return application->r_code;
}

SEXP r_instrumentr_application_get_code(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    return instrumentr_application_get_code(application);
}

/*******************************************************************************
 * r_environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_application_get_environment(
    instrumentr_application_t application) {
    return application->r_environment;
}

SEXP r_instrumentr_application_get_environment(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    return instrumentr_application_get_environment(application);
}

/*******************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor  */
int instrumentr_application_get_frame_position(
    instrumentr_application_t application) {
    return application->frame_position;
}

SEXP r_instrumentr_application_get_frame_position(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    int frame_position =
        instrumentr_application_get_frame_position(application);
    return instrumentr_c_int_to_r_integer(frame_position);
}

/*******************************************************************************
 * call_stack
 *******************************************************************************/

/* accessor  */
instrumentr_call_stack_t
instrumentr_application_get_call_stack(instrumentr_application_t application) {
    return application->call_stack;
}

SEXP r_instrumentr_application_get_call_stack(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_call_stack_t call_stack =
        instrumentr_application_get_call_stack(application);
    return instrumentr_call_stack_wrap(call_stack);
}

/*******************************************************************************
 * packages
 *******************************************************************************/

/* accessor  */
int instrumentr_application_get_package_count(
    instrumentr_application_t application) {
    return application->packages.length;
}

SEXP r_instrumentr_application_get_package_count(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    int count = instrumentr_application_get_package_count(application);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_package_t instrumentr_application_get_package_by_position(
    instrumentr_application_t application,
    int position) {
    int count = instrumentr_application_get_package_count(application);
    if (position < count && position >= 0) {
        return application->packages.data[position];
    } else {
        instrumentr_log_error(
            "attempt to access %d package of a application with %d packages",
            position,
            count);
        /* NOTE: never executed */
        return NULL;
    }
}

SEXP r_instrumentr_application_get_package_by_position(SEXP r_application,
                                                       SEXP r_position) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_package_t package =
        instrumentr_application_get_package_by_position(application, position);
    return instrumentr_package_wrap(package);
}

/* accessor  */
instrumentr_package_t instrumentr_application_get_package_by_name(
    instrumentr_application_t application,
    const char* name) {
    int count = instrumentr_application_get_package_count(application);
    instrumentr_package_t* packages = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        const char* actual_name = instrumentr_package_get_name(package);
        if (actual_name != NULL && strcmp(actual_name, name) == 0) {
            return package;
        }
    }

    instrumentr_log_error(
        "package with name '%s' does not exist for this application", name);
    /* NOTE: never executed */
    return NULL;
}

SEXP r_instrumentr_application_get_package_by_name(SEXP r_application,
                                                   SEXP r_name) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_package_t package =
        instrumentr_application_get_package_by_name(application, name);
    return instrumentr_package_wrap(package);
}

/* accessor */
instrumentr_package_t instrumentr_application_get_base_package(
    instrumentr_application_t application) {
    return instrumentr_application_get_package_by_name(application, "base");
}

SEXP r_instrumentr_application_get_base_package(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package =
        instrumentr_application_get_base_package(application);
    return instrumentr_package_wrap(package);
}

/* accessor */
SEXP r_instrumentr_application_get_packages(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t* packages = application->packages.data;
    int count = instrumentr_application_get_package_count(application);

    SEXP r_packages = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        const char* name = instrumentr_package_get_name(package);
        SET_VECTOR_ELT(r_packages, i, instrumentr_package_wrap(package));
        SET_STRING_ELT(r_names, i, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_packages, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_packages;
}

/* mutator */
void instrumentr_application_add_package(instrumentr_application_t application,
                                         instrumentr_package_t package) {
    vec_push(&application->packages, package);

    int fun_count = instrumentr_package_get_function_count(package);
}

/* mutator */
SEXP r_instrumentr_application_add_package(SEXP r_application, SEXP r_package) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_application_add_package(application, package);
    return R_NilValue;
}

/* mutator */
void instrumentr_application_remove_package(
    instrumentr_application_t application,
    instrumentr_package_t package) {
    vec_remove(&application->packages, package);

    int fun_count = instrumentr_package_get_function_count(package);

    for (int i = 0; i < fun_count; ++i) {
        instrumentr_function_t function =
            instrumentr_package_get_function_by_position(package, i);

        SEXP r_closure = instrumentr_function_get_definition(function).sexp;

        instrumentr_application_function_map_remove(application, r_closure);
    }
}

SEXP r_instrumentr_application_remove_package(SEXP r_application,
                                              SEXP r_package) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);

    instrumentr_application_remove_package(application, package);

    return R_NilValue;
}

instrumentr_function_t instrumentr_application_function_map_lookup(
    instrumentr_state_t state,
    instrumentr_application_t application,
    SEXP r_closure,
    SEXP r_call) {
    instrumentr_function_t function = NULL;

    auto result = application->function_map->find(r_closure);

    /* the function is already present */
    if (result != application->function_map->end()) {
        function = result->second;
    }
    /* create the function and add to cache if function is not present */
    else {
        function = instrumentr_application_function_map_add(
            state, application, r_closure);
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

instrumentr_function_t instrumentr_application_function_map_insert(
    instrumentr_application_t application,
    instrumentr_function_t function,
    SEXP r_closure) {
    auto result = application->function_map->insert({r_closure, function});

    bool inserted = result.second;

    if (!inserted) {
        instrumentr_object_release(result.first->second);
        result.first->second = function;

        // TODO: uncommenting this causes errors. fix them!
        // instrumentr_log_error(
        //    "unable to insert a new function object to function map, perhaps "
        //    "the cache has not been cleaned of old objects?");
    }

    return function;
}

void instrumentr_application_function_map_remove(
    instrumentr_application_t application,
    SEXP r_closure) {
    auto result = application->function_map->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != application->function_map->end()) {
        instrumentr_object_release(result->second);
        application->function_map->erase(r_closure);
    }
}

instrumentr_function_t
instrumentr_application_function_map_add(instrumentr_state_t state,
                                         instrumentr_application_t application,
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
            instrumentr_application_get_call_stack(application);
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

    return instrumentr_application_function_map_insert(
        application, function, r_closure);
}

void instrumentr_application_function_map_update_name(
    instrumentr_state_t state,
    instrumentr_application_t application,
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

    auto result = application->function_map->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != application->function_map->end()) {
        function = result->second;
        if (!instrumentr_function_has_name(function)) {
            instrumentr_function_set_name(function, name);
        }
    }
    /* this means the function is alien */
    else {
        int alien = 1;
        instrumentr_function_t parent = NULL;

        /* TODO: memory management - function will not be collected if insert
         * fails. */
        function =
            instrumentr_function_create_closure(state,
                                                name,
                                                Rf_length(CAR(r_closure)),
                                                r_closure,
                                                parent,
                                                0,
                                                0,
                                                0 /*, alien, parent*/);

        instrumentr_application_function_map_insert(
            application, function, r_closure);
    }
}

/* NOTE: this function is called from package load hook for top level package
 * functions. It updates function properties and adds them to package */
SEXP r_instrumentr_application_function_map_update_properties(
    SEXP r_state,
    SEXP r_application,
    SEXP r_package,
    SEXP r_name,
    SEXP r_closure,
    SEXP r_rho,
    SEXP r_pub,
    SEXP r_s3_generic,
    SEXP r_s3_method) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);

    instrumentr_function_t parent = NULL;
    int pub = asLogical(r_pub);
    int s3_generic = asLogical(r_s3_generic);
    int s3_method = asLogical(r_s3_method);

    const char* name = CHAR(STRING_ELT(r_name, 0));

    instrumentr_function_t function = NULL;

    auto result = application->function_map->find(r_closure);

    /* this means the function is already present (it is not alien) */
    if (result != application->function_map->end()) {
        function = result->second;
        instrumentr_function_set_name(function, name);
        instrumentr_function_set_public(function, pub);
        instrumentr_function_set_s3_generic(function, s3_generic);
        instrumentr_function_set_s3_method(function, s3_method);
    }
    /* this means the function is alien */
    else {
        /* TODO: memory management - function will not be collected if insert
         * fails. */
        function =
            instrumentr_function_create_closure(state,
                                                name,
                                                Rf_length(CAR(r_closure)),
                                                r_closure,
                                                parent,
                                                pub,
                                                s3_generic,
                                                s3_method /*, alien, */);

        instrumentr_application_function_map_insert(
            application, function, r_closure);
    }

    instrumentr_package_t package = instrumentr_package_unwrap(r_package);

    if (!instrumentr_package_has_function(package, name)) {
        instrumentr_package_add_function(package, function);
    }

    return R_NilValue;
}
