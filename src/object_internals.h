#ifndef INSTRUMENTR_OBJECT_INTERNALS_H
#define INSTRUMENTR_OBJECT_INTERNALS_H

#include <instrumentr/object.h>

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
    INSTRUMENTR_ARGUMENT
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

#endif /* INSTRUMENTR_OBJECT_INTERNALS_H */
