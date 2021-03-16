#ifndef INSTRUMENTR_MODEL_H
#define INSTRUMENTR_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

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

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_model_get_id(void* model);
SEXP r_instrumentr_model_get_id(SEXP r_model);

/*******************************************************************************
 * type
 *******************************************************************************/

instrumentr_model_type_t instrumentr_model_get_type(void* model);

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_model_acquire(void* model);

/* mutator  */
int instrumentr_model_release(void* model);

int instrumentr_model_get_reference_count(void* model);

int instrumentr_model_kill(void* model);

/*******************************************************************************
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_model_get_birth_time(void* model);
SEXP r_instrumentr_model_get_birth_time(SEXP r_model);

/* accessor */
int instrumentr_model_get_death_time(void* model);
SEXP r_instrumentr_model_get_death_time(SEXP r_model);

/* accessor */
int instrumentr_model_get_life_time(void* model);
SEXP r_instrumentr_model_get_life_time(SEXP r_model);

/*******************************************************************************
 * lifetime
 *******************************************************************************/

/* accessor */
int instrumentr_model_is_alive(void* model);
SEXP r_instrumentr_model_is_alive(SEXP r_model);

/* accessor */
int instrumentr_model_is_dead(void* model);
SEXP r_instrumentr_model_is_dead(SEXP r_model);

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_model_is_local(void* model);
SEXP r_instrumentr_model_is_local(SEXP r_model);

/* accessor */
int instrumentr_model_is_foreign(void* model);
SEXP r_instrumentr_model_is_foreign(SEXP r_model);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_model_wrap(instrumentr_model_t model);
instrumentr_model_t instrumentr_model_unwrap(SEXP r_model,
                                             instrumentr_model_type_t type);

#define INSTRUMENTR_MODEL_INTEROP_DECLARE_API(MODEL_NAME, MODEL_TYPE) \
    SEXP instrumentr_##MODEL_NAME##_wrap(                             \
        instrumentr_##MODEL_NAME##_t MODEL_NAME);                     \
    instrumentr_##MODEL_NAME##_t instrumentr_##MODEL_NAME##_unwrap(   \
        SEXP r_##MODEL_NAME);

#define INSTRUMENTR_MODEL_INTEROP_DEFINE_API(MODEL_NAME, MODEL_TYPE)      \
    SEXP instrumentr_##MODEL_NAME##_wrap(                                 \
        instrumentr_##MODEL_NAME##_t MODEL_NAME) {                        \
        return instrumentr_model_wrap((instrumentr_model_t)(MODEL_NAME)); \
    }                                                                     \
                                                                          \
    instrumentr_##MODEL_NAME##_t instrumentr_##MODEL_NAME##_unwrap(       \
        SEXP r_##MODEL_NAME) {                                            \
        instrumentr_model_t model =                                       \
            instrumentr_model_unwrap(r_##MODEL_NAME, MODEL_TYPE);         \
        return (instrumentr_##MODEL_NAME##_t)(model);                     \
    }

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MODEL_H */
