#include "object_internals.h"
#include <instrumentr/object.h>
#include "interop.h"
#include "utilities.h"

static int object_counter = 0;

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

void r_instrumentr_object_finalize(SEXP r_object) {
    void* object = instrumentr_r_externalptr_to_c_pointer(r_object);
    if (object == NULL) {
        instrumentr_log_error(
            "instrumentr object finalizer encountered NULL object in externalptr");
    }
    R_ClearExternalPtr(r_object);
    instrumentr_object_release(object);
}

SEXP instrumentr_object_wrap(instrumentr_object_t object) {
    SEXP r_object = PROTECT(instrumentr_c_pointer_to_r_externalptr(object, NULL));
    instrumentr_object_acquire(object);
    R_RegisterCFinalizerEx(r_object, r_instrumentr_object_finalize, 1);
    instrumentr_sexp_set_class(r_object, instrumentr_object_get_class(object->type));
    UNPROTECT(1);
    return r_object;
}

instrumentr_object_t
instrumentr_object_unwrap(SEXP r_object, instrumentr_object_type_t type) {
    instrumentr_object_t object =
        instrumentr_r_externalptr_to_c_pointer(r_object);
    if (type == INSTRUMENTR_OBJECT || object->type == type) {
        return object;
    } else {
        instrumentr_log_error(
            "cannot unwrap %d object as %d object", object->type, type);
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

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_object_get_id(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj->id;
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
int instrumentr_object_has_c_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj->c_data != NULL;
}

SEXP r_instrumentr_object_has_c_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_has_c_data(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
void* instrumentr_object_get_c_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    if (instrumentr_object_has_c_data(obj)) {
        return obj->c_data;
    } else {
        instrumentr_log_error("object does not have C data");
        /* NOTE: not executed  */
        return NULL;
    }
}

SEXP r_instrumentr_object_get_c_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    void* c_data = instrumentr_object_get_c_data(object);
    return instrumentr_c_pointer_to_r_externalptr(c_data, NULL);
}

/* mutator  */
void instrumentr_object_set_c_data(void* object, void* c_data) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    obj->c_data = c_data;
}

SEXP r_instrumentr_object_set_c_data(SEXP r_object, SEXP r_c_data) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    void* c_data = instrumentr_r_externalptr_to_c_pointer(r_c_data);
    instrumentr_object_set_c_data(object, c_data);
    return R_NilValue;
}

/* mutator  */
void instrumentr_object_remove_c_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    obj->c_data = NULL;
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
int instrumentr_object_has_r_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj->r_data != NULL;
}

SEXP r_instrumentr_object_has_r_data(SEXP r_object) {
    void* object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_has_r_data(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
SEXP instrumentr_object_get_r_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    if (instrumentr_object_has_r_data(obj)) {
        return obj->r_data;
    } else {
        instrumentr_log_error("object does not have R data");
        /* NOTE: not executed  */
        return NULL;
    }
}

SEXP r_instrumentr_object_get_r_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    return instrumentr_object_get_r_data(object);
}

/* mutator  */
void instrumentr_object_set_r_data(void* object, SEXP r_data) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    instrumentr_object_remove_r_data(obj);
    instrumentr_sexp_acquire(r_data);
    obj->r_data = r_data;
}

SEXP r_instrumentr_object_set_r_data(SEXP r_object, SEXP r_data) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_set_r_data(object, r_data);
    return R_NilValue;
}

/* mutator  */
void instrumentr_object_remove_r_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    if (instrumentr_object_has_r_data(obj)) {
        instrumentr_sexp_release(obj->r_data);
        obj->r_data = NULL;
    }
}

SEXP r_instrumentr_object_remove_r_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_remove_r_data(object);
    return R_NilValue;
}
