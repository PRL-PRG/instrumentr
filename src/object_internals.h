#ifndef INSTRUMENTR_OBJECT_INTERNALS_H
#define INSTRUMENTR_OBJECT_INTERNALS_H

enum instrumentr_type_t {
    INSTRUMENTR_OBJECT = 0,
    INSTRUMENTR_APPLICATION,
    INSTRUMENTR_PACKAGE,
    INSTRUMENTR_FUNCTION,
    INSTRUMENTR_CALL,
    INSTRUMENTR_PARAMETER,
    INSTRUMENTR_ARGUMENT
};

struct instrumentr_object_impl_t {
    instrumentr_id_t id;
    enum instrumentr_object_type_t type;
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
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size,
                          enum instrumentr_object_type_t type,
                          instrumentr_object_finalizer_t finalizer);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_object_wrap(instrumentr_object_t object);
instrumentr_object_t instrumentr_object_unwrap(SEXP r_object);

#endif /* INSTRUMENTR_OBJECT_INTERNALS_H */
