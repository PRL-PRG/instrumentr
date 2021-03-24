#ifndef INSTRUMENTR_MODEL_H
#define INSTRUMENTR_MODEL_H

#include <instrumentr/api.h>
#include "model_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*instrumentr_model_finalizer_t)(instrumentr_model_t model);

/*******************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_model_impl_t {
    instrumentr_id_t id;
    instrumentr_model_type_t type;
    int reference_count;
    instrumentr_model_finalizer_t finalizer;
    int birth_time;
    int death_time;
    instrumentr_origin_t origin;
    instrumentr_state_t state;
};

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_model_t
instrumentr_model_create(instrumentr_state_t state,
                         int size,
                         instrumentr_model_type_t type,
                         instrumentr_model_finalizer_t finalizer,
                         instrumentr_origin_t origin);

int instrumentr_model_acquire(instrumentr_model_t model);

int instrumentr_model_release(instrumentr_model_t model);

int instrumentr_model_kill(instrumentr_model_t model);

void r_instrumentr_model_release(SEXP r_model);

/*******************************************************************************
 * state
 *******************************************************************************/

instrumentr_state_t instrumentr_model_get_state(instrumentr_model_t model);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_model_wrap(instrumentr_model_t model);
instrumentr_model_t instrumentr_model_unwrap(SEXP r_model,
                                             instrumentr_model_type_t type);

/*******************************************************************************
 * type
 *******************************************************************************/

instrumentr_model_type_t instrumentr_model_get_type(instrumentr_model_t model);

#define INSTRUMENTR_MODEL_DEFINE_DERIVED_API(                                 \
    MODEL_TYPE, MODEL_NAME, MODEL_VAR)                                        \
    SEXP instrumentr_##MODEL_NAME##_wrap(                                     \
        instrumentr_##MODEL_NAME##_t MODEL_NAME##_var) {                      \
        return instrumentr_model_wrap(                                        \
            (instrumentr_model_t)(MODEL_NAME##_var));                         \
    }                                                                         \
                                                                              \
    instrumentr_##MODEL_NAME##_t instrumentr_##MODEL_NAME##_unwrap(           \
        SEXP r_##MODEL_NAME) {                                                \
        instrumentr_model_t model =                                           \
            instrumentr_model_unwrap(r_##MODEL_NAME, MODEL_TYPE);             \
        return (instrumentr_##MODEL_NAME##_t)(model);                         \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_acquire(                                   \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_acquire(model);                              \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_release(                                   \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_release(model);                              \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_kill(                                      \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_kill(model);                                 \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_get_reference_count(                       \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_reference_count(model);                  \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_get_reference_count(SEXP r_model) {     \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int reference_count =                                                 \
            instrumentr_##MODEL_NAME##_get_reference_count(MODEL_VAR);        \
        return instrumentr_c_int_to_r_integer(reference_count);               \
    }                                                                         \
    instrumentr_id_t instrumentr_##MODEL_NAME##_get_id(                       \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_id(model);                               \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_get_id(SEXP r_model) {                  \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        instrumentr_id_t id = instrumentr_##MODEL_NAME##_get_id(MODEL_VAR);   \
        return instrumentr_c_int_to_r_integer(id);                            \
    }                                                                         \
    bool instrumentr_model_is_##MODEL_NAME(instrumentr_model_t model) {       \
        return instrumentr_model_get_type(model) == MODEL_TYPE;               \
    }                                                                         \
    instrumentr_##MODEL_NAME##_t instrumentr_model_as_##MODEL_NAME(           \
        instrumentr_model_t model) {                                          \
        instrumentr_model_type_t actual = instrumentr_model_get_type(model);  \
        if (actual != MODEL_TYPE) {                                           \
            const char* actual_str = instrumentr_model_type_get_name(actual); \
            const char* expected_str =                                        \
                instrumentr_model_type_get_name(MODEL_TYPE);                  \
            instrumentr_log_error("cannot return model of type %s as %s",     \
                                  actual_str,                                 \
                                  expected_str);                              \
        }                                                                     \
        return (instrumentr_##MODEL_NAME##_t)(model);                         \
    }                                                                         \
    instrumentr_model_t instrumentr_##MODEL_NAME##_as_model(                  \
        instrumentr_##MODEL_NAME##_t MODEL_NAME##_var) {                      \
        return (instrumentr_model_t)(MODEL_NAME##_var);                       \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_get_birth_time(                            \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_birth_time(model);                       \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_get_birth_time(SEXP r_model) {          \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_get_birth_time(MODEL_VAR);    \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_get_death_time(                            \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_death_time(model);                       \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_get_death_time(SEXP r_model) {          \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_get_death_time(MODEL_VAR);    \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_get_life_time(                             \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_life_time(model);                        \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_get_life_time(SEXP r_model) {           \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_get_life_time(MODEL_VAR);     \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_is_alive(                                  \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_is_alive(model);                             \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_is_alive(SEXP r_model) {                \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_is_alive(MODEL_VAR);          \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_is_dead(                                   \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_is_dead(model);                              \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_is_dead(SEXP r_model) {                 \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_is_dead(MODEL_VAR);           \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_is_local(                                  \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_is_local(model);                             \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_is_local(SEXP r_model) {                \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_is_local(MODEL_VAR);          \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##MODEL_NAME##_is_foreign(                                \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_is_foreign(model);                           \
    }                                                                         \
    SEXP r_instrumentr_##MODEL_NAME##_is_foreign(SEXP r_model) {              \
        instrumentr_##MODEL_NAME##_t MODEL_VAR =                              \
            instrumentr_##MODEL_NAME##_unwrap(r_model);                       \
        int result = instrumentr_##MODEL_NAME##_is_foreign(MODEL_VAR);        \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    instrumentr_state_t instrumentr_##MODEL_NAME##_get_state(                 \
        instrumentr_##MODEL_NAME##_t MODEL_VAR) {                             \
        instrumentr_model_t model = (instrumentr_model_t)(MODEL_VAR);         \
        return instrumentr_model_get_state(model);                            \
    }

INSTRUMENTR_MODEL_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MODEL_H */
