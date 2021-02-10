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
    void* c_data;
    SEXP r_data;
};

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_object_class_initialize();

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_class_finalize();

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_get_class(instrumentr_object_type_t object_type);

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size,
                          instrumentr_object_type_t type,
                          instrumentr_object_finalizer_t finalizer);

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
 * r_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_data(void* object);
SEXP r_instrumentr_object_has_data(SEXP r_object);

/* mutator  */
void instrumentr_object_set_data(void* object, SEXP r_data);
SEXP r_instrumentr_object_set_data(SEXP r_object, SEXP r_data);

/* accessor */
SEXP instrumentr_object_get_data(void* object);
SEXP r_instrumentr_object_get_data(SEXP r_object);

/* mutator  */
void instrumentr_object_remove_data(void* object);
SEXP r_instrumentr_object_remove_data(SEXP r_object);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_OBJECT_H */
