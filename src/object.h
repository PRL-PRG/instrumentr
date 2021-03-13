#ifndef INSTRUMENTR_OBJECT_H
#define INSTRUMENTR_OBJECT_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*instrumentr_object_finalizer_t)(instrumentr_object_t object);

/*******************************************************************************
 * definition
 *******************************************************************************/

typedef enum {
    INSTRUMENTR_OBJECT = 0,
    INSTRUMENTR_TRACER,
    INSTRUMENTR_CALLBACK,
    INSTRUMENTR_STATE,
    INSTRUMENTR_APPLICATION,
    INSTRUMENTR_PACKAGE,
    INSTRUMENTR_FUNCTION,
    INSTRUMENTR_CALL,
    INSTRUMENTR_CALL_STACK,
    INSTRUMENTR_PARAMETER,
    INSTRUMENTR_ARGUMENT,
    INSTRUMENTR_PROMISE,
    INSTRUMENTR_VALUE,
    INSTRUMENTR_FRAME,
    INSTRUMENTR_CONTEXT,
    INSTRUMENTR_EXEC_STATS
} instrumentr_object_type_t;

struct instrumentr_object_impl_t {
    instrumentr_id_t id;
    instrumentr_object_type_t type;
    int reference_count;
    instrumentr_object_finalizer_t finalizer;
    int birth_time;
    int death_time;
    instrumentr_origin_t origin;
};

/*******************************************************************************
 * kill
 *******************************************************************************/

void instrumentr_object_kill(instrumentr_object_t object,
                             instrumentr_state_t state);

/*******************************************************************************
 * object class
 *******************************************************************************/

void instrumentr_object_class_initialize();
void instrumentr_object_class_finalize();

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_get_class(instrumentr_object_type_t object_type);

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t instrumentr_object_create(int size);

void instrumentr_object_initialize(instrumentr_object_t object,
                                   instrumentr_state_t state,
                                   instrumentr_object_type_t type,
                                   instrumentr_object_finalizer_t finalizer,
                                   instrumentr_origin_t origin);

instrumentr_object_t instrumentr_object_create_and_initialize(
    int size,
    instrumentr_state_t state,
    instrumentr_object_type_t type,
    instrumentr_object_finalizer_t finalizer,
    instrumentr_origin_t origin);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_object_wrap(instrumentr_object_t object);
instrumentr_object_t instrumentr_object_unwrap(SEXP r_object,
                                               instrumentr_object_type_t type);

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_object_get_id(void* object);
SEXP r_instrumentr_object_get_id(SEXP r_object);

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_object_acquire(void* object);

/* mutator  */
int instrumentr_object_release(void* object);

/*******************************************************************************
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_object_get_birth_time(instrumentr_object_t object);
SEXP r_instrumentr_object_get_birth_time(SEXP r_object);

/* accessor */
int instrumentr_object_get_death_time(instrumentr_object_t object);
SEXP r_instrumentr_object_get_death_time(SEXP r_object);

/* accessor */
int instrumentr_object_get_life_time(instrumentr_object_t object);
SEXP r_instrumentr_object_get_life_time(SEXP r_object);

/*******************************************************************************
 * alive
 *******************************************************************************/

/* accessor */
int instrumentr_object_is_alive(instrumentr_object_t object);
SEXP r_instrumentr_object_is_alive(SEXP r_object);

/* accessor */
int instrumentr_object_is_dead(instrumentr_object_t object);
SEXP r_instrumentr_object_is_dead(SEXP r_object);

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_object_is_local(instrumentr_object_t object);
SEXP r_instrumentr_object_is_local(SEXP r_object);

/* accessor */
int instrumentr_object_is_foreign(instrumentr_object_t object);
SEXP r_instrumentr_object_is_foreign(SEXP r_object);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_OBJECT_H */
