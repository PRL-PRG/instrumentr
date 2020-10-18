#include "callback.h"
#include "object.h"
#include "vec.h"
#include "interop.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef union {
    void* c;
    SEXP r;
} function_t;

struct instrumentr_callback_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_callback_type_t type;
    function_t function;
    int has_r_function;
    int active;
    instrumentr_exec_stats_t exec_stats;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_callback_finalize(instrumentr_object_t object) {
    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    if (instrumentr_callback_has_r_function(callback)) {
        instrumentr_sexp_release(callback->function.r);
    }
    callback->function.r = NULL;

    instrumentr_exec_stats_destroy(callback->exec_stats);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_callback_t
instrumentr_callback_create(instrumentr_callback_t callback,
                            instrumentr_callback_type_t type) {
    callback->type = type;

    callback->active = 0;

    callback->exec_stats = instrumentr_exec_stats_create();

    return callback;
}

instrumentr_callback_t
instrumentr_callback_create_from_r_function(instrumentr_callback_type_t type,
                                            SEXP r_function) {
    const char* name = instrumentr_callback_type_get_name(type);

    int expected_parameter_count =
        instrumentr_callback_type_get_parameter_count(type);

    int actual_parameter_count = Rf_length(FORMALS(r_function));

    if (actual_parameter_count != expected_parameter_count) {
        instrumentr_log_error("callback %s expects a %d parameter R function "
                              "but received a %d parameter function",
                              name,
                              expected_parameter_count,
                              actual_parameter_count);
    }

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_callback_impl_t),
                                  INSTRUMENTR_CALLBACK,
                                  instrumentr_callback_finalize);

    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    instrumentr_sexp_acquire(r_function);
    callback->function.r = r_function;

    callback->has_r_function = 1;

    return instrumentr_callback_create(callback, type);
}

instrumentr_callback_t
instrumentr_callback_create_from_c_function(instrumentr_callback_type_t type,
                                            void* c_function) {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_callback_impl_t),
                                  INSTRUMENTR_CALLBACK,
                                  instrumentr_callback_finalize);

    instrumentr_callback_t callback = (instrumentr_callback_t)(object);

    callback->function.c = c_function;

    callback->has_r_function = 0;

    return instrumentr_callback_create(callback, type);
}

#define CALLBACK_CREATE(TYPE, NAME, ...)                                      \
    instrumentr_callback_t                                                    \
        instrumentr_callback_##NAME##_create_from_r_function(                 \
            SEXP r_function) {                                                \
        return instrumentr_callback_create_from_r_function(TYPE, r_function); \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_callback_##NAME##_create_from_r_function(              \
        SEXP r_function) {                                                    \
        instrumentr_callback_t callback =                                     \
            instrumentr_callback_##NAME##_create_from_r_function(r_function); \
        return instrumentr_callback_wrap(callback);                           \
    }                                                                         \
                                                                              \
    instrumentr_callback_t                                                    \
        instrumentr_callback_##NAME##_create_from_c_function(                 \
            NAME##_function_t c_function) {                                   \
        return instrumentr_callback_create_from_c_function(                   \
            TYPE, (void*) (c_function));                                      \
    }                                                                         \
                                                                              \
    SEXP r_instrumentr_callback_##NAME##_create_from_c_function(              \
        SEXP r_c_function) {                                                  \
        void* c_function =                                                    \
            instrumentr_r_externalptr_to_c_pointer(r_c_function);             \
        instrumentr_callback_t callback =                                     \
            instrumentr_callback_##NAME##_create_from_c_function(c_function); \
        return instrumentr_callback_wrap(callback);                           \
    }

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(CALLBACK_CREATE)

#undef CALLBACK_CREATE

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_callback_wrap(instrumentr_callback_t callback) {
    return instrumentr_object_wrap((instrumentr_object_t)(callback));
}

instrumentr_callback_t instrumentr_callback_unwrap(SEXP r_callback) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_callback, INSTRUMENTR_CALLBACK);
    return (instrumentr_callback_t)(object);
}

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_callback_type_t
instrumentr_callback_get_type(instrumentr_callback_t callback) {
    return callback->type;
}

SEXP r_instrumentr_callback_get_type(SEXP r_callback) {
    return r_instrumentr_callback_get_name(r_callback);
}

#define INSTRUMENTR_GENERATE_DEFINITION_CALLBACK_IS_TYPE(TYPE, NAME, ...) \
    /* accessor  */                                                       \
    int instrumentr_callback_is_##NAME(instrumentr_callback_t callback) { \
        return instrumentr_callback_get_type(callback) == TYPE;           \
    }                                                                     \
    SEXP r_instrumentr_callback_is_##NAME(SEXP r_callback) {              \
        instrumentr_callback_t callback =                                 \
            instrumentr_callback_unwrap(r_callback);                      \
        int result = instrumentr_callback_is_##NAME(callback);            \
        return instrumentr_c_int_to_r_logical(result);                    \
    }

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(
    INSTRUMENTR_GENERATE_DEFINITION_CALLBACK_IS_TYPE)

#undef INSTRUMENTR_GENERATE_DEFINITION_CALLBACK_IS_TYPE

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_callback_get_name(instrumentr_callback_t callback) {
    instrumentr_callback_type_t callback_type =
        instrumentr_callback_get_type(callback);
    return instrumentr_callback_type_get_name(callback_type);
}

SEXP r_instrumentr_callback_get_name(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    const char* name = instrumentr_callback_get_name(callback);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_get_parameter_count(instrumentr_callback_t callback) {
    instrumentr_callback_type_t callback_type =
        instrumentr_callback_get_type(callback);
    return instrumentr_callback_type_get_parameter_count(callback_type);
}

SEXP r_instrumentr_callback_get_parameter_count(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int parameter_count = instrumentr_callback_get_parameter_count(callback);
    return instrumentr_c_int_to_r_integer(parameter_count);
}

/********************************************************************************
 * r_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_r_function(instrumentr_callback_t callback) {
    return callback->has_r_function;
}

SEXP r_instrumentr_callback_has_r_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_has_r_function(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_callback_get_r_function(instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        return callback->function.r;
    } else {
        const char* type_str = instrumentr_callback_get_name(callback);
        instrumentr_log_error("R function not set for callback %s", type_str);
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_callback_get_r_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    SEXP r_function = instrumentr_callback_get_r_function(callback);
    return r_function;
}

/********************************************************************************
 * c_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_c_function(instrumentr_callback_t callback) {
    return !callback->has_r_function;
}

SEXP r_instrumentr_callback_has_c_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_has_c_function(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
void* instrumentr_callback_get_c_function(instrumentr_callback_t callback) {
    if (instrumentr_callback_has_r_function(callback)) {
        const char* type_str = instrumentr_callback_get_name(callback);
        instrumentr_log_error("C function not set for callback %s", type_str);
        /* NOTE: not executed */
        return NULL;
    } else {
        return callback->function.c;
    }
}

SEXP r_instrumentr_callback_get_c_function(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    void* c_function = instrumentr_callback_get_c_function(callback);
    return instrumentr_c_pointer_to_r_externalptr(
        c_function, R_NilValue, R_NilValue, NULL);
}

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_is_active(instrumentr_callback_t callback) {
    return callback->active;
}

SEXP r_instrumentr_callback_is_active(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    int result = instrumentr_callback_is_active(callback);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator  */
void instrumentr_callback_activate(instrumentr_callback_t callback) {
    callback->active = 1;
}

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback) {
    callback->active = 0;
}

/********************************************************************************
 * exec_stats
 *******************************************************************************/

/* accessor  */
instrumentr_exec_stats_t
instrumentr_callback_get_exec_stats(instrumentr_callback_t callback) {
    return callback->exec_stats;
}

/* accessor  */
SEXP r_instrumentr_callback_get_exec_stats(SEXP r_callback) {
    instrumentr_callback_t callback = instrumentr_callback_unwrap(r_callback);
    instrumentr_exec_stats_t exec_stats =
        instrumentr_callback_get_exec_stats(callback);
    return instrumentr_exec_stats_wrap(exec_stats);
}
