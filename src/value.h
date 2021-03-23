#ifndef INSTRUMENTR_VALUE_H
#define INSTRUMENTR_VALUE_H

#include <instrumentr/api.h>
#include "value_type.h"
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*instrumentr_value_finalizer_t)(instrumentr_value_t value);

/*******************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_value_impl_t {
    instrumentr_model_impl_t model;
    instrumentr_value_finalizer_t finalizer;
    instrumentr_value_type_t type;
    SEXP r_sexp;
};

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t
instrumentr_value_create(instrumentr_state_t state,
                         int size,
                         instrumentr_value_type_t type,
                         instrumentr_value_finalizer_t finalizer,
                         instrumentr_origin_t origin,
                         SEXP r_sexp);

instrumentr_value_type_t instrumentr_value_get_type(instrumentr_value_t value);

int instrumentr_value_acquire(instrumentr_value_t value);

int instrumentr_value_release(instrumentr_value_t value);

int instrumentr_value_kill(instrumentr_value_t value);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_value_wrap(instrumentr_value_t value);
instrumentr_value_t instrumentr_value_unwrap(SEXP r_value,
                                             instrumentr_value_type_t type);

/*******************************************************************************
 * reference_count
 *******************************************************************************/

int instrumentr_value_get_reference_count(instrumentr_value_t value);

SEXP r_instrumentr_value_get_reference_count(SEXP r_value);

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_value_get_id(instrumentr_value_t value);

SEXP r_instrumentr_value_get_id(SEXP r_value);

/*******************************************************************************
 * type
 *******************************************************************************/

instrumentr_value_type_t instrumentr_value_get_type(instrumentr_value_t value);

/*******************************************************************************
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_value_get_birth_time(instrumentr_value_t value);

SEXP r_instrumentr_value_get_birth_time(SEXP r_value);

/* accessor */
int instrumentr_value_get_death_time(instrumentr_value_t value);

SEXP r_instrumentr_value_get_death_time(SEXP r_value);

/* accessor */
int instrumentr_value_get_life_time(instrumentr_value_t value);

SEXP r_instrumentr_value_get_life_time(SEXP r_value);

/*******************************************************************************
 * alive
 *******************************************************************************/

/* accessor */
int instrumentr_value_is_alive(instrumentr_value_t value);

SEXP r_instrumentr_value_is_alive(SEXP r_value);

/* accessor */
int instrumentr_value_is_dead(instrumentr_value_t value);

SEXP r_instrumentr_value_is_dead(SEXP r_value);

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_value_is_local(instrumentr_value_t value);

SEXP r_instrumentr_value_is_local(SEXP r_value);

/* accessor */
int instrumentr_value_is_foreign(instrumentr_value_t value);

SEXP r_instrumentr_value_is_foreign(SEXP r_value);

/*******************************************************************************
 * state
 *******************************************************************************/

/* accessor */
instrumentr_state_t instrumentr_value_get_state(instrumentr_value_t value);

/*******************************************************************************
 * r_sexp
 *******************************************************************************/

/* accessor */
SEXP instrumentr_value_get_sexp(instrumentr_value_t value);

SEXP r_instrumentr_value_get_sexp(SEXP r_value);

#define INSTRUMENTR_VALUE_DECLARE_API(VALUE_TYPE, VALUE_NAME, VALUE_VAR)     \
    SEXP instrumentr_##VALUE_NAME##_wrap(                                    \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    instrumentr_##VALUE_NAME##_t instrumentr_##VALUE_NAME##_unwrap(          \
        SEXP r_##VALUE_NAME);                                                \
    int instrumentr_##VALUE_NAME##_acquire(                                  \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    int instrumentr_##VALUE_NAME##_release(                                  \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    int instrumentr_##VALUE_NAME##_kill(                                     \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    int instrumentr_##VALUE_NAME##_get_reference_count(                      \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_reference_count(                   \
        SEXP r_##VALUE_NAME##_var);                                          \
    instrumentr_id_t instrumentr_##VALUE_NAME##_get_id(                      \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_id(SEXP r_##VALUE_NAME##_var);     \
    bool instrumentr_value_is_##VALUE_NAME(instrumentr_value_t value);       \
    instrumentr_##VALUE_NAME##_t instrumentr_value_as_##VALUE_NAME(          \
        instrumentr_value_t value);                                          \
    int instrumentr_##VALUE_NAME##_get_birth_time(                           \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_birth_time(                        \
        SEXP r_##VALUE_NAME##_var);                                          \
    int instrumentr_##VALUE_NAME##_get_death_time(                           \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_death_time(                        \
        SEXP r_##VALUE_NAME##_var);                                          \
    int instrumentr_##VALUE_NAME##_get_life_time(                            \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_life_time(                         \
        SEXP r_##VALUE_NAME##_var);                                          \
    int instrumentr_##VALUE_NAME##_is_alive(                                 \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_is_alive(SEXP r_##VALUE_NAME##_var);   \
    int instrumentr_##VALUE_NAME##_is_dead(                                  \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_is_dead(SEXP r_##VALUE_NAME##_var);    \
    int instrumentr_##VALUE_NAME##_is_local(                                 \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_is_local(SEXP r_##VALUE_NAME##_var);   \
    int instrumentr_##VALUE_NAME##_is_foreign(                               \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_is_foreign(SEXP r_##VALUE_NAME##_var); \
    instrumentr_state_t instrumentr_##VALUE_NAME##_get_state(                \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP instrumentr_##VALUE_NAME##_get_sexp(                                \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var);                      \
    SEXP r_instrumentr_##VALUE_NAME##_get_sexp(SEXP r_##VALUE_NAME##_var);

#define INSTRUMENTR_VALUE_DEFINE_API(VALUE_TYPE, VALUE_NAME, VALUE_VAR)       \
    SEXP instrumentr_##VALUE_NAME##_wrap(                                     \
        instrumentr_##VALUE_NAME##_t VALUE_NAME##_var) {                      \
        return instrumentr_value_wrap(                                        \
            (instrumentr_value_t)(VALUE_NAME##_var));                         \
    }                                                                         \
                                                                              \
    instrumentr_##VALUE_NAME##_t instrumentr_##VALUE_NAME##_unwrap(           \
        SEXP r_##VALUE_NAME) {                                                \
        instrumentr_value_t value =                                           \
            instrumentr_value_unwrap(r_##VALUE_NAME, VALUE_TYPE);             \
        return (instrumentr_##VALUE_NAME##_t)(value);                         \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_acquire(                                   \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_acquire(value);                              \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_release(                                   \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_release(value);                              \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_kill(                                      \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_kill(value);                                 \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_get_reference_count(                       \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_reference_count(value);                  \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_reference_count(SEXP r_value) {     \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int reference_count =                                                 \
            instrumentr_##VALUE_NAME##_get_reference_count(VALUE_VAR);        \
        return instrumentr_c_int_to_r_integer(reference_count);               \
    }                                                                         \
    instrumentr_id_t instrumentr_##VALUE_NAME##_get_id(                       \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_id(value);                               \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_id(SEXP r_value) {                  \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        instrumentr_id_t id = instrumentr_##VALUE_NAME##_get_id(VALUE_VAR);   \
        return instrumentr_c_int_to_r_integer(id);                            \
    }                                                                         \
    bool instrumentr_value_is_##VALUE_NAME(instrumentr_value_t value) {       \
        return instrumentr_value_get_type(value) == VALUE_TYPE;               \
    }                                                                         \
    instrumentr_##VALUE_NAME##_t instrumentr_value_as_##VALUE_NAME(           \
        instrumentr_value_t value) {                                          \
        instrumentr_value_type_t actual = instrumentr_value_get_type(value);  \
        if (actual != VALUE_TYPE) {                                           \
            const char* actual_str = instrumentr_value_type_get_name(actual); \
            const char* expected_str =                                        \
                instrumentr_value_type_get_name(VALUE_TYPE);                  \
            instrumentr_log_error("cannot return value of type %s as %s",     \
                                  actual_str,                                 \
                                  expected_str);                              \
        }                                                                     \
        return (instrumentr_##VALUE_NAME##_t)(value);                         \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_get_birth_time(                            \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_birth_time(value);                       \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_birth_time(SEXP r_value) {          \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_get_birth_time(VALUE_VAR);    \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_get_death_time(                            \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_death_time(value);                       \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_death_time(SEXP r_value) {          \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_get_death_time(VALUE_VAR);    \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_get_life_time(                             \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_life_time(value);                        \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_life_time(SEXP r_value) {           \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_get_life_time(VALUE_VAR);     \
        return instrumentr_c_int_to_r_integer(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_is_alive(                                  \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_is_alive(value);                             \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_is_alive(SEXP r_value) {                \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_is_alive(VALUE_VAR);          \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_is_dead(                                   \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_is_dead(value);                              \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_is_dead(SEXP r_value) {                 \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_is_dead(VALUE_VAR);           \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_is_local(                                  \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_is_local(value);                             \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_is_local(SEXP r_value) {                \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_is_local(VALUE_VAR);          \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    int instrumentr_##VALUE_NAME##_is_foreign(                                \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_is_foreign(value);                           \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_is_foreign(SEXP r_value) {              \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        int result = instrumentr_##VALUE_NAME##_is_foreign(VALUE_VAR);        \
        return instrumentr_c_int_to_r_logical(result);                        \
    }                                                                         \
    instrumentr_state_t instrumentr_##VALUE_NAME##_get_state(                 \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_state(value);                            \
    }                                                                         \
    SEXP instrumentr_##VALUE_NAME##_get_sexp(                                 \
        instrumentr_##VALUE_NAME##_t VALUE_VAR) {                             \
        instrumentr_value_t value = (instrumentr_value_t)(VALUE_VAR);         \
        return instrumentr_value_get_sexp(value);                             \
    }                                                                         \
    SEXP r_instrumentr_##VALUE_NAME##_get_sexp(SEXP r_value) {                \
        instrumentr_##VALUE_NAME##_t VALUE_VAR =                              \
            instrumentr_##VALUE_NAME##_unwrap(r_value);                       \
        return instrumentr_##VALUE_NAME##_get_sexp(VALUE_VAR);                \
    }

INSTRUMENTR_VALUE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_VALUE_H */
