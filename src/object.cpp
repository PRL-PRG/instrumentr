#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object_type.h"

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size,
                          instrumentr_object_type_t type,
                          instrumentr_object_finalizer_t finalizer) {
    instrumentr_object_t object = (instrumentr_object_t) calloc(1, size);
    if (object == NULL) {
        Rf_error("allocation of new instrumentr object failed");
    }

    object->type = type;
    object->reference_count = 1;
    object->finalizer = finalizer;

    return object;
}

/*******************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_object_destroy(instrumentr_object_t object) {
    if (object->finalizer == NULL) {
        Rf_error("attempt to finalize already finalized object");
    }
    object->finalizer(object);
    object->finalizer = NULL;
    free(object);
}

/********************************************************************************
 * interop
 *******************************************************************************/

void r_instrumentr_object_finalize(SEXP r_object) {
    void* object = instrumentr_r_externalptr_to_c_pointer(r_object);
    if (object == NULL) {
        instrumentr_log_error("instrumentr object finalizer encountered NULL "
                              "object in externalptr");
    }
    R_ClearExternalPtr(r_object);
    instrumentr_object_release(object);
}

SEXP instrumentr_object_wrap(instrumentr_object_t object) {
    SEXP r_object = instrumentr_c_pointer_to_r_externalptr(
        object, R_NilValue, R_NilValue, r_instrumentr_object_finalize);
    PROTECT(r_object);
    instrumentr_object_acquire(object);
    instrumentr_sexp_set_class(r_object,
                               instrumentr_object_type_get_class(object->type));
    UNPROTECT(1);
    return r_object;
}

instrumentr_object_t instrumentr_object_unwrap(SEXP r_object,
                                               instrumentr_object_type_t type) {
    instrumentr_object_t object =
        (instrumentr_object_t) instrumentr_r_externalptr_to_c_pointer(r_object);
    if (type == INSTRUMENTR_OBJECT_TYPE_COUNT || object->type == type) {
        return object;
    } else {
        instrumentr_log_error("cannot unwrap %s object as %s object",
                              instrumentr_object_type_get_name(object->type),
                              instrumentr_object_type_get_name(type));
        /* NOTE: not executed  */
        return NULL;
    }
}

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_object_acquire(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    obj->reference_count += 1;
    return obj->reference_count;
}

/* mutator  */
int instrumentr_object_release(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    obj->reference_count -= 1;
    int reference_count = obj->reference_count;

    if (reference_count == 0) {
        instrumentr_object_destroy(obj);
    }

    return reference_count;
}

int instrumentr_object_get_reference_count(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj->reference_count;
}

SEXP r_instrumentr_object_get_reference_count(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT_TYPE_COUNT);

    int reference_count = instrumentr_object_get_reference_count(object);

    return instrumentr_c_int_to_r_integer(reference_count);
}
