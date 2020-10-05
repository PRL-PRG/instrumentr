#include "object_internals.h"
#include <instrumentr/object.h>

static int object_counter = 0;

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size,
                          enum instrumentr_object_type_t type,
                          instrumentr_object_finalizer_t finalizer) {
    instrumentr_object_t object = (instrumentr_object_t) std::calloc(1, size);

    if (object == NULL) {
        Rf_error("allocation of new instrumentr object failed");
    }

    object->id = object_counter++;
    object->type = type;
    object->reference_count = 1;
    object->finalizer = finalizer;
    object->c_data = NULL;
    object->r_data = NULL;

    return object;
}

/*******************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_object_destroy(instrumentr_object_t object) {
    object->finalizer(object);

    instrumentr_object_remove_r_data(object);
    instrumentr_object_remove_c_data(object);
    object->finalizer = NULL;
    object->id = -1;

    free(object);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_object_wrap(instrumentr_object_t object) {
    SEXP r_object = instrumentr_c_pointer_to_r_externalptr(
        object, instrumentr_object_decrement_reference);
    instrumentr_sexp_set_class(instrumentr_object_type_class[object->type]);
    return r_object;
}

instrumentr_object_t
instrumentr_object_unwrap(SEXP r_object, enum instrumentr_object_type_t type) {
    instrumentr_object_t object =
        instrumentr_r_externalptr_to_c_pointer(r_object);
    if (type == INSTRUMENTR_OBJECT || object->type == type) {
        return object;
    } else {
        instrumentr_raise_error(
            "cannot unwrap %d object as %d object", object->type, type);
    }
}

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_object_increment_reference(instrumentr_object_t object) {
    object->reference_count += 1;
    return object->reference_count;
}

/* mutator  */
int instrumentr_object_decrement_reference(instrumentr_object_t object) {
    object->reference_count -= 1;
    int reference_count = object->reference_count;

    if (reference_count == 0) {
        instrumentr_object_destroy(object);
    }

    return reference_count;
}

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_object_get_id(instrumentr_object_t object) {
    return object->id;
}

SEXP r_instrumentr_object_get_id(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_id_t id = instrumentr_object_get_id(object);
    return instrumentr_c_int_to_r_integer(id);
}

/*******************************************************************************
 * c_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_c_data(instrumentr_object_t object) {
    return object->c_data != NULL;
}

SEXP r_instrumentr_object_has_c_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_has_c_data(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
void* instrumentr_object_get_c_data(instrumentr_object_t object) {
    if (instrumentr_object_has_c_data(object)) {
        return object->c_data;
    } else {
        instrumentr_raise_error("object does not have C data");
    }
}

SEXP r_instrumentr_object_get_c_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    void* c_data = instrumentr_object_get_c_data(object);
    return instrumentr_c_pointer_to_r_externalptr(c_data);
}

/* mutator  */
void instrumentr_object_set_c_data(instrumentr_object_t object, SEXP c_data) {
    object->c_data = c_data;
}

SEXP r_instrumentr_object_set_c_data(SEXP r_object, SEXP r_c_data) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    void* c_data = instrumentr_r_externalptr_to_c_pointer(r_c_data);
    instrumentr_object_set_c_data(object, c_data);
    return R_NilValue;
}

/* mutator  */
void instrumentr_object_remove_c_data(instrumentr_object_t object) {
    object->c_data = NULL;
}

SEXP r_instrumentr_object_remove_c_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_remove_c_data(object);
    return R_NilValue;
}

/*******************************************************************************
 * r_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_r_data(instrumentr_object_t object) {
    return object->r_data != NULL;
}

SEXP r_instrumentr_object_has_r_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_has_r_data(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
SEXP instrumentr_object_get_r_data(instrumentr_object_t object) {
    if (instrumentr_object_has_r_data(object)) {
        return object->r_data;
    } else {
        instrumentr_raise_error("object does not have R data");
    }
}

SEXP r_instrumentr_object_get_r_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    return instrumentr_object_get_r_data(object);
}

/* mutator  */
void instrumentr_object_set_r_data(instrumentr_object_t object, SEXP r_data) {
    instrumentr_object_remove_r_data(object);
    instrumentr_sexp_acquire(r_data);
    object->r_data = r_data;
}

SEXP r_instrumentr_object_set_r_data(SEXP r_object, SEXP r_data) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_set_r_data(object, r_data);
    return R_NilValue;
}

/* mutator  */
void instrumentr_object_remove_r_data(instrumentr_object_t object) {
    if (instrumentr_object_has_r_data(object)) {
        instrumentr_sexp_release(object->r_data);
        object->r_data = NULL;
    }
}

SEXP r_instrumentr_object_remove_r_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_remove_r_data(object);
    return R_NilValue;
}
