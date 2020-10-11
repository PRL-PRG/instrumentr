#include "object_internals.h"
#include <instrumentr/tracer.h>
#include <instrumentr/callback.h>
#include <instrumentr/application.h>
#include "vec.h"

#ifdef USING_DYNTRACE
#    include "rdyntrace.h"
#endif

/********************************************************************************
 * definition
 *******************************************************************************/

typedef struct package_t {
    const char* name;
    vec_str_t functions;
}

struct instrumentr_tracer_impl_t {
    instrumentr_object_impl_t object;

#ifdef USING_DYNTRACE
    dyntracer_t* dyntracer;
#endif /* USING_DYNTRACE */

    instrumentr_application_t application;
    SEXP r_environment;
    instrumentr_callback_t active_callback;
    vec_int_t status;
    vec_t(package_t) packages;

    typedef struct callbacks_t {
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

    instrumentr_object_decrement_reference(tracer->application);
    tracer->application = NULL;

    instrumentr_sexp_release(tracer->r_environment);
    tracer->r_environment = NULL;

    tracer->active_callback = NULL;

    vec_deinit(&tracer->status);

    int package_count = tracer->packages.length;
    package_t packages[] = tracer->packages.data;

    for (int i = 0; i < package_count; ++i) {
        free(tracer->packages[i].name);

        int function_count = tracer->packages[i].functions.length;
        const char* functions[] = tracer->packages[i].functions.data;

        for (int j = 0; j < function_count; ++j) {
            free(functions[j]);
        }

        vec_deinit(&tracer->packages[i].functions);
    }

    vec_deinit(&tracer->packages);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_tracer_t
instrumentr_tracer_create(instrumentr_application_t application) {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_tracer_impl_t),
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

    tracer->application = application;
    instrumentr_object_increment_reference(application);

    SEXP r_environment = Rf_NewEnvironment(R_NilValue, R_NilValue, R_EmptyEnv);
    tracer->r_environment = r_environment;
    instrumentr_sexp_acquire(r_environment);

    tracer->active_callback = NULL;

    vec_init(&tracer->status);

    vec_init(tracer->packages);

#define INITIALIZE_CALLBACK(TYPE, NAME) tracer->callbacks.NAME = NULL;

    INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(INITIALIZE_CALLBACK)

#undef INITIALIZE_CALLBACK

    return tracer;
}

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
}

SEXP r_instrumentr_tracer_enable(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_enable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_disable(instrumentr_tracer_t tracer) {
    vec_push(&tracer->status, 0);
}

void r_instrumentr_tracer_disable(instrumentr_tracer_t tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_disable(tracer);
    return R_NilValue;
}

/* mutator  */
void instrumentr_tracer_reinstate(instrumentr_tracer_t tracer) {
    if (tracer->status.length != 0) {
        vec_pop(&tracer->status);
    }
}

void r_instrumentr_tracer_reinstate(instrumentr_tracer_t tracer) {
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
 * packages
 *******************************************************************************/

int get_traced_package_index(instrumentr_tracer_t tracer,
                             const char* package_name) {
    int count = tracer->packages.length;
    package_t packages[] = tracer->packages.data;

    for (int i = 0; i < count; ++i) {
        if (strcmp(name, packages[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

int get_traced_function_index(instrumentr_tracer_t tracer,
                              int package_index,
                              const char* function_name) {
    package_t package = packages[i];

    int function_count = package.functions.length;
    const char* functions[] = package.functions.data;

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
    int count = tracer->packages.length;
    package_t packages[] = tracer->packages.data;

    for (int i = 0; i < count; ++i) {
        if (strcmp(name, packages[i].name) == 0) {
            return packages[i].functions.length;
        }
    }

    instrumentr_raise_error("package %s is not being traced", package_name);
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
    const char* name = instrumentr_r_character_to_c_string(r_name);
    int result = instrumentr_tracer_is_package_traced(tracer, name);
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

    int count = tracer->packages.length;
    package_t packages[] = tracer->packages.data;

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

    for (int i = 0; i < package_count; ++i) {
        package_t package = packages[i];

        if (strcmp(package_name, package.name)) {
            int function_count = package.functions.length;
            const char* functions[] = package.functions.data;

            SEXP r_function_names =
                PROTECT(allocVector(STRSXP, function_count));

            for (int j = 0; j < function_count; ++j) {
                SET_STRING_ELT(r_function_names, j, mkChar(functions[j]));
            }

            UNPROTECT(1);

            return r_function_names;
        }
    }

    instrumentr_raise_error("package %s is not traced", package_name);
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

SEXP r_tracer_trace_function(SEXP r_tracer,
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
        R_RemoveVarFromFrame(Rf_install(callback_name),
                             instrumentr_tracer_get_environment(tracer));
    }
}

#define TRACER_CALLBACK_API(TYPE, NAME)                                        \
                                                                               \
    /* accessor */                                                             \
    int instrumentr_tracer_has_##NAME##_callback(instrumentr_trace_t tracer) { \
        return tracer->callbacks.NAME != NULL;                                 \
    }                                                                          \
                                                                               \
    SEXP r_instrumentr_tracer_has_##NAME##_callback(SEXP r_tracer) {           \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);     \
        int result = instrumentr_tracer_has_##NAME##_callback(tracer);         \
        return instrumentr_c_int_to_r_logical(result);                         \
    }                                                                          \
                                                                               \
    /* accessor */                                                             \
    instrumentr_callback_t instrumentr_tracer_get_##NAME##_callback(           \
        instrumentr_trace_t tracer) {                                          \
        if (instrumentr_tracer_has_##NAME##_callback(tracer)) {                \
            return tracer->callbacks.NAME;                                     \
        } else {                                                               \
            instrumentr_raise_error("tracer does not have a %s callback",      \
                                    #NAME);                                    \
        }                                                                      \
    }                                                                          \
                                                                               \
    SEXP r_instrumentr_tracer_get_##NAME##_callback(SEXP r_tracer) {           \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);     \
        instrumentr_callback_t callback =                                      \
            instrumentr_tracer_get_##NAME##_callback(tracer);                  \
        return instrumentr_callback_wrap(callback);                            \
    }                                                                          \
                                                                               \
    /* accessor */                                                             \
    void instrumentr_tracer_set_##NAME##_callback(                             \
        instrumentr_trace_t tracer, instrumentr_callback_t callback) {         \
        if (instrumentr_callback_is_##NAME(callback)) {                        \
            instrumentr_tracer_remove_##NAME##_callback(tracer);               \
            tracer->callbacks.NAME = callback;                                 \
            instrumentr_object_increment_reference(callback)                   \
                bind_callback(tracer, callback);                               \
        } else {                                                               \
            instrumentr_raise_error(                                           \
                "attempting to attach %s callback as %s callback",             \
                instrumentr_callback_get_name(callback),                       \
                ##NAME);                                                       \
        }                                                                      \
    }                                                                          \
                                                                               \
    SEXP r_instrumentr_tracer_set_##NAME##_callback(SEXP r_tracer,             \
                                                    SEXP r_callback) {         \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);     \
        instrumentr_callback_t callback =                                      \
            instrumentr_callback_unwrap(r_callback);                           \
        return instrumentr_tracer_set_##NAME##_callback(tracer, callback);     \
    }                                                                          \
                                                                               \
    /* mutator */                                                              \
    void instrumentr_tracer_remove_##NAME##_callback(                          \
        instrumentr_trace_t tracer) {                                          \
        if (instrumentr_tracer_has_##NAME##_callback(tracer)) {                \
            unbind_callback(tracer, tracer->callbacks.NAME);                   \
            instrumentr_object_decrement_reference(tracer->callbacks.NAME);    \
        }                                                                      \
                                                                               \
        tracer->callbacks.NAME = NULL;                                         \
    }                                                                          \
    SEXP r_instrumentr_tracer_remove_##NAME##_callback(SEXP r_tracer) {        \
        instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);     \
        instrumentr_tracer_remove_##NAME##_callback(tracer);                   \
        return R_NilValue;                                                     \
    }

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_API)

#undef TRACER_CALLBACK_API
#undef INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO
