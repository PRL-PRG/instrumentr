#include <instrumentr/Rincludes.h>
#include "object_internals.h"
#include <instrumentr/tracer.h>
#include "tracer_internals.h"
#include <instrumentr/callback.h>
#include <instrumentr/application.h>
#include "vec.h"
#include "interop.h"
#include "utilities.h"
#include "dyntrace.h"
#include "dyntracer.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef struct {
    const char* name;
    vec_str_t functions;
} package_t;

struct instrumentr_tracer_impl_t {
    struct instrumentr_object_impl_t object;

#ifdef USING_DYNTRACE
    dyntracer_t* dyntracer;
#endif /* USING_DYNTRACE */

    instrumentr_application_t application;
    SEXP r_environment;
    instrumentr_callback_t active_callback;
    vec_int_t status;
    vec_t(package_t) packages;

    struct callbacks_t {
#define DECLARE_CALLBACK(TYPE, NAME) instrumentr_callback_t NAME;

        INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(DECLARE_CALLBACK)

#undef DECLARE_CALLBACK
    } callbacks;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_tracer_finalize(instrumentr_object_t object) {
    instrumentr_tracer_t tracer = (instrumentr_tracer_t)(object);

#ifdef USING_DYNTRACE
    instrumentr_dyntracer_destroy(tracer->dyntracer);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This creates a cyclic reference, tracer holds a reference to itself via
       dyntracer.
    */
    tracer->dyntracer = NULL;
#endif

    if(tracer->application != NULL) {
        instrumentr_object_release(tracer->application);
        tracer->application = NULL;
    }

    instrumentr_sexp_release(tracer->r_environment);
    tracer->r_environment = NULL;

    tracer->active_callback = NULL;

    vec_deinit(&tracer->status);

    int package_count = tracer->packages.length;
    package_t* packages = tracer->packages.data;

    for (int i = 0; i < package_count; ++i) {
        free((char*) (tracer->packages.data[i].name));

        int function_count = tracer->packages.data[i].functions.length;
        char** functions = tracer->packages.data[i].functions.data;

        for (int j = 0; j < function_count; ++j) {
            free(functions[j]);
        }

        vec_deinit(&(tracer->packages.data[i].functions));
    }

    vec_deinit(&tracer->packages);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_tracer_t instrumentr_tracer_create() {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_tracer_impl_t),
                                  INSTRUMENTR_TRACER,
                                  instrumentr_tracer_finalize);

    instrumentr_tracer_t tracer = (instrumentr_tracer_t)(object);

#ifdef USING_DYNTRACE
    dyntracer_t* dyntracer = instrumentr_dyntracer_create(tracer);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This creates a cyclic reference, tracer holds a reference to itself via
       dyntracer.
    */
    tracer->dyntracer = dyntracer;

#endif

    tracer->application = NULL;

    SEXP r_environment = create_environment(ScalarLogical(1), R_EmptyEnv);
    tracer->r_environment = r_environment;
    instrumentr_sexp_acquire(r_environment);

    tracer->active_callback = NULL;

    vec_init(&tracer->status);

    vec_init(&tracer->packages);

#define INITIALIZE_CALLBACK(TYPE, NAME) tracer->callbacks.NAME = NULL;

    INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(INITIALIZE_CALLBACK)

#undef INITIALIZE_CALLBACK

    return tracer;
}

SEXP r_instrumentr_tracer_create() {
    instrumentr_tracer_t tracer = instrumentr_tracer_create();
    SEXP r_tracer = instrumentr_tracer_wrap(tracer);
    instrumentr_object_release(tracer);
    return r_tracer;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_tracer_wrap(instrumentr_tracer_t tracer) {
    return instrumentr_object_wrap((instrumentr_object_t)(tracer));
}

instrumentr_tracer_t instrumentr_tracer_unwrap(SEXP r_tracer) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_tracer, INSTRUMENTR_TRACER);
    return (instrumentr_tracer_t)(object);
}

/********************************************************************************
 * dyntracer
 *******************************************************************************/

#ifdef USING_DYNTRACE

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer) {
    return tracer->dyntracer;
}

#endif

/********************************************************************************
 * application
 *******************************************************************************/

/*  accessor  */
instrumentr_application_t
instrumentr_tracer_get_application(instrumentr_tracer_t tracer) {
    return tracer->application;
}

SEXP r_instrumentr_tracer_get_application(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);
    return instrumentr_application_wrap(application);
}

/*  mutator  */
void instrumentr_tracer_set_application(instrumentr_tracer_t tracer,
                                        instrumentr_application_t application) {
    tracer->application = application;
    instrumentr_object_acquire(application);
}

/*  mutator  */
void instrumentr_tracer_remove_application(instrumentr_tracer_t tracer) {
    instrumentr_object_release(tracer->application);
    tracer->application = NULL;
}

/********************************************************************************
 * r_environment
 *******************************************************************************/

/*  accessor  */
SEXP instrumentr_tracer_get_environment(instrumentr_tracer_t tracer) {
    return tracer->r_environment;
}

SEXP r_instrumentr_tracer_get_environment(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    return instrumentr_tracer_get_environment(tracer);
}

/********************************************************************************
 * active_callback
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_active(instrumentr_tracer_t tracer) {
    return tracer->active_callback != NULL;
}

SEXP r_instrumentr_tracer_is_active(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    int result = instrumentr_tracer_is_active(tracer);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_callback_t
instrumentr_tracer_get_active_callback(instrumentr_tracer_t tracer) {
    return tracer->active_callback;
}

SEXP r_instrumentr_tracer_get_active_callback(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_callback_t callback =
        instrumentr_tracer_get_active_callback(tracer);
    return instrumentr_callback_wrap(callback);
}

/* mutator  */
void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback) {
    if (tracer->active_callback != NULL) {
        instrumentr_log_error(
            "attempt to set active callback when it is already set");
    } else {
        tracer->active_callback = callback;
        instrumentr_object_acquire(callback);
    }
}

/* mutator  */
void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer) {
    if (tracer->active_callback != NULL) {
        instrumentr_object_release(tracer->active_callback);
        tracer->active_callback = NULL;
    }
}

/********************************************************************************
 * status
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_enabled(instrumentr_tracer_t tracer) {
    return (tracer->status.length != 0) && vec_last(&tracer->status);
}

SEXP r_instrumentr_tracer_is_enabled(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    int result = instrumentr_tracer_is_enabled(tracer);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator  */
void instrumentr_tracer_enable(instrumentr_tracer_t tracer) {
    vec_push(&tracer->status, 1);
#ifdef USING_DYNTRACE
    dyntrace_enable();
#endif
}

SEXP r_instrumentr_tracer_enable(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_enable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_disable(instrumentr_tracer_t tracer) {
    vec_push(&tracer->status, 0);
#ifdef USING_DYNTRACE
    dyntrace_disable();
#endif
}

SEXP r_instrumentr_tracer_disable(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_disable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_reinstate(instrumentr_tracer_t tracer) {
    if (tracer->status.length != 0) {
        int result = vec_pop(&tracer->status);
    }
#ifdef USING_DYNTRACE
    if (instrumentr_tracer_is_enabled(tracer)) {
        dyntrace_enable();
    } else {
        dyntrace_disable();
    }
#endif
}

SEXP r_instrumentr_tracer_reinstate(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_reinstate(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status) {
    vec_push(&tracer->status, status);
}

/********************************************************************************
 * reset
 *******************************************************************************/

void instrumentr_tracer_reset(instrumentr_tracer_t tracer) {
    instrumentr_tracer_unset_active_callback(tracer);
    vec_clear(&tracer->status);
}

/********************************************************************************
 * packages
 *******************************************************************************/

int get_traced_package_index(instrumentr_tracer_t tracer,
                             const char* package_name) {
    int count = tracer->packages.length;
    package_t* packages = tracer->packages.data;

    for (int i = 0; i < count; ++i) {
        if (strcmp(package_name, packages[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

int get_traced_function_index(instrumentr_tracer_t tracer,
                              int package_index,
                              const char* function_name) {
    package_t package = tracer->packages.data[package_index];

    int function_count = package.functions.length;
    char** functions = package.functions.data;

    for (int j = 0; j < function_count; ++j) {
        if (strcmp(function_name, functions[j]) == 0) {
            return j;
        }
    }

    return -1;
}

int instrumentr_tracer_get_traced_package_count(instrumentr_tracer_t tracer) {
    return tracer->packages.length;
}

SEXP r_instrumentr_tracer_get_traced_package_count(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    int result = instrumentr_tracer_get_traced_package_count(tracer);
    return instrumentr_c_int_to_r_integer(result);
}

int instrumentr_tracer_get_traced_function_count(instrumentr_tracer_t tracer,
                                                 const char* package_name) {
    int index = get_traced_package_index(tracer, package_name);

    if (index == -1) {
        instrumentr_log_error("package %s is not being traced", package_name);
        /* NOTE: not executed  */
        return -1;
    } else {
        package_t package = tracer->packages.data[index];
        return package.functions.length;
    }
}

SEXP r_instrumentr_tracer_get_traced_function_count(SEXP r_tracer,
                                                    SEXP r_package_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);
    int result =
        instrumentr_tracer_get_traced_function_count(tracer, package_name);
    return instrumentr_c_int_to_r_integer(result);
}

int instrumentr_tracer_is_package_traced(instrumentr_tracer_t tracer,
                                         const char* name) {
    return get_traced_package_index(tracer, name) != -1;
}

SEXP r_instrumentr_tracer_is_package_traced(SEXP r_tracer,
                                            SEXP r_package_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);
    int result = instrumentr_tracer_is_package_traced(tracer, package_name);
    return instrumentr_c_int_to_r_logical(result);
}

int instrumentr_tracer_is_function_traced(instrumentr_tracer_t tracer,
                                          const char* package_name,
                                          const char* function_name) {
    int package_index = get_traced_package_index(tracer, package_name);

    if (package_index == -1) {
        return 0;
    }

    int function_index =
        get_traced_function_index(tracer, package_index, function_name);

    return function_index != -1;
}

SEXP r_instrumentr_tracer_is_function_traced(SEXP r_tracer,
                                             SEXP r_package_name,
                                             SEXP r_function_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);
    const char* function_name =
        instrumentr_r_character_to_c_string(r_function_name);
    int result = instrumentr_tracer_is_function_traced(
        tracer, package_name, function_name);
    return instrumentr_c_int_to_r_logical(result);
}

SEXP r_instrumentr_tracer_get_traced_packages(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    int package_count = tracer->packages.length;
    package_t* packages = tracer->packages.data;

    SEXP r_packages = PROTECT(allocVector(STRSXP, package_count));

    for (int index = 0; index < package_count; ++index) {
        SET_STRING_ELT(r_packages, index, mkChar(packages[index].name));
    }

    UNPROTECT(1);

    return r_packages;
}

SEXP r_instrumentr_tracer_get_traced_functions(SEXP r_tracer,
                                               SEXP r_package_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);

    int index = get_traced_package_index(tracer, package_name);

    if (index == -1) {
        instrumentr_log_error("package %s is not traced", package_name);
        /* NOTE: not executed  */
        return R_NilValue;
    } else {
        package_t package = tracer->packages.data[index];
        int function_count = package.functions.length;
        char** functions = package.functions.data;

        SEXP r_function_names = PROTECT(allocVector(STRSXP, function_count));

        for (int j = 0; j < function_count; ++j) {
            SET_STRING_ELT(r_function_names, j, mkChar(functions[j]));
        }

        UNPROTECT(1);

        return r_function_names;
    }
}

void instrumentr_tracer_trace_package(instrumentr_tracer_t tracer,
                                      const char* package_name) {
    int package_index = get_traced_package_index(tracer, package_name);

    if (package_index == -1) {
        package_t package;
        package.name = instrumentr_duplicate_string(package_name);
        vec_init(&package.functions);
        vec_push(&tracer->packages, package);
    }
}

SEXP r_instrumentr_tracer_trace_package(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);
    instrumentr_tracer_trace_package(tracer, package_name);
    return R_NilValue;
}

void instrumentr_tracer_trace_function(instrumentr_tracer_t tracer,
                                       const char* package_name,
                                       const char* function_name) {
    int package_index = get_traced_package_index(tracer, package_name);

    if (package_index == -1) {
        package_t package;
        package.name = instrumentr_duplicate_string(package_name);
        vec_init(&package.functions);
        vec_push(&package.functions,
                 instrumentr_duplicate_string(function_name));
        vec_push(&tracer->packages, package);
        return;
    }

    int function_index =
        get_traced_function_index(tracer, package_index, function_name);

    if (function_index == -1) {
        vec_push(&tracer->packages.data[package_index].functions,
                 instrumentr_duplicate_string(function_name));
    }
}

SEXP r_instrumentr_tracer_trace_function(SEXP r_tracer,
                                         SEXP r_package_name,
                                         SEXP r_function_name) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    const char* package_name =
        instrumentr_r_character_to_c_string(r_package_name);
    const char* function_name =
        instrumentr_r_character_to_c_string(r_function_name);
    instrumentr_tracer_trace_function(tracer, package_name, function_name);
    return R_NilValue;
}

/********************************************************************************
 * callbacks
 *******************************************************************************/

void bind_callback(instrumentr_tracer_t tracer,
                   instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        const char* callback_name = instrumentr_callback_get_name(callback);
        Rf_defineVar(Rf_install(callback_name),
                     instrumentr_callback_get_r_function(callback),
                     instrumentr_tracer_get_environment(tracer));
    }
}

void unbind_callback(instrumentr_tracer_t tracer,
                     instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        const char* callback_name = instrumentr_callback_get_name(callback);
        R_removeVarFromFrame(Rf_install(callback_name),
                             instrumentr_tracer_get_environment(tracer));
    }
}

#define TRACER_CALLBACK_API(TYPE, NAME)                                       \
                                                                              \
    /* accessor */                                                            \
    int instrumentr_tracer_has_callback_##NAME(instrumentr_tracer_t tracer) { \
        return tracer->callbacks.NAME != NULL;                                \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_tracer_has_callback_##NAME(SEXP r_tracer) {            \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);    \
        int result = instrumentr_tracer_has_callback_##NAME(tracer);          \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
                                                                              \
    /* accessor */                                                            \
    instrumentr_callback_t instrumentr_tracer_get_callback_##NAME(            \
        instrumentr_tracer_t tracer) {                                        \
        if (instrumentr_tracer_has_callback_##NAME(tracer)) {                 \
            return tracer->callbacks.NAME;                                    \
        } else {                                                              \
            instrumentr_log_error("tracer does not have a %s callback",       \
                                  #NAME);                                     \
            /* NOTE: not executed */                                          \
            return NULL;                                                      \
        }                                                                     \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_tracer_get_callback_##NAME(SEXP r_tracer) {            \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);    \
        instrumentr_callback_t callback =                                     \
            instrumentr_tracer_get_callback_##NAME(tracer);                   \
        return instrumentr_callback_wrap(callback);                           \
    }                                                                         \
                                                                              \
    /* accessor */                                                            \
    void instrumentr_tracer_set_callback_##NAME(                              \
        instrumentr_tracer_t tracer, instrumentr_callback_t callback) {       \
        if (instrumentr_callback_is_##NAME(callback)) {                       \
            instrumentr_tracer_remove_callback_##NAME(tracer);                \
            tracer->callbacks.NAME = callback;                                \
            instrumentr_object_acquire(callback);                             \
            bind_callback(tracer, callback);                                  \
        } else {                                                              \
            instrumentr_log_error(                                            \
                "attempting to attach %s callback as %s callback",            \
                instrumentr_callback_get_name(callback),                      \
                #NAME);                                                       \
        }                                                                     \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_tracer_set_callback_##NAME(SEXP r_tracer,              \
                                                  SEXP r_callback) {          \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);    \
        instrumentr_callback_t callback =                                     \
            instrumentr_callback_unwrap(r_callback);                          \
        instrumentr_tracer_set_callback_##NAME(tracer, callback);             \
        return R_NilValue;                                                    \
    }                                                                         \
                                                                              \
    /* mutator */                                                             \
    void instrumentr_tracer_remove_callback_##NAME(                           \
        instrumentr_tracer_t tracer) {                                        \
        if (instrumentr_tracer_has_callback_##NAME(tracer)) {                 \
            unbind_callback(tracer, tracer->callbacks.NAME);                  \
            instrumentr_object_release(tracer->callbacks.NAME);               \
        }                                                                     \
                                                                              \
        tracer->callbacks.NAME = NULL;                                        \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_tracer_remove_callback_##NAME(SEXP r_tracer) {         \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);    \
        instrumentr_tracer_remove_callback_##NAME(tracer);                    \
        return R_NilValue;                                                    \
    }

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_API)

#undef TRACER_CALLBACK_API
