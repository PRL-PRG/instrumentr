#include "object.h"
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
    object->birth_time = -1; /* TODO */
    object->death_time = -1; /* TODO */
    object->local = 0;       /* TODO */
    object->r_data = NULL;

    return object;
}

/*******************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_object_destroy(instrumentr_object_t object) {
    object->finalizer(object);

    instrumentr_object_remove_data(object);
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
    SEXP r_object = instrumentr_c_pointer_to_r_externalptr(
        object, R_NilValue, R_NilValue, r_instrumentr_object_finalize);
    PROTECT(r_object);
    instrumentr_object_acquire(object);
    instrumentr_sexp_set_class(r_object,
                               instrumentr_object_get_class(object->type));
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
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_object_get_birth_time(instrumentr_object_t object) {
    return object -> birth_time;
}

SEXP r_instrumentr_object_get_birth_time(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_get_birth_time(object);
    return instrumentr_c_int_to_r_integer(result);
}

/* accessor */
int instrumentr_object_get_death_time(instrumentr_object_t object) {
    return object -> death_time;
}

SEXP r_instrumentr_object_get_death_time(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_get_death_time(object);
    return instrumentr_c_int_to_r_integer(result);
}

/* accessor */
int instrumentr_object_get_life_time(instrumentr_object_t object) {
    if(object -> death_time < 0) {
        return object -> death_time;
    }
    else if(object -> birth_time < 0) {
        return object -> birth_time;
    }
    else {
        return object->death_time - object->birth_time;
    }
}

SEXP r_instrumentr_object_get_life_time(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_get_life_time(object);
    return instrumentr_c_int_to_r_integer(result);
}

/*******************************************************************************
 * alive
 *******************************************************************************/

/* accessor */
int instrumentr_object_is_alive(instrumentr_object_t object) {
    return !(object -> death_time > 0);
}

SEXP r_instrumentr_object_is_alive(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_alive(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
int instrumentr_object_is_dead(instrumentr_object_t object) {
    return !instrumentr_object_is_alive(object);
}

SEXP r_instrumentr_object_is_dead(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_dead(object);
    return instrumentr_c_int_to_r_logical(result);
}

void instrumentr_object_kill(instrumentr_object_t object, int time) {
    object -> death_time = time;
}

/*******************************************************************************
 * local
 *******************************************************************************/

/* accessor */
int instrumentr_object_is_local(instrumentr_object_t object) {
    return object -> local;
}

SEXP r_instrumentr_object_is_local(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_local(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
int instrumentr_object_is_foreign(instrumentr_object_t object) {
    return !instrumentr_object_is_local(object);
}

SEXP r_instrumentr_object_is_foreign(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_foreign(object);
    return instrumentr_c_int_to_r_logical(result);
}



/*******************************************************************************
 * r_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj->r_data != NULL;
}

SEXP r_instrumentr_object_has_data(SEXP r_object) {
    void* object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_has_data(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
SEXP instrumentr_object_get_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    if (instrumentr_object_has_data(obj)) {
        return obj->r_data;
    } else {
        instrumentr_log_error("object does not have R data");
        /* NOTE: not executed  */
        return NULL;
    }
}

SEXP r_instrumentr_object_get_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    return instrumentr_object_get_data(object);
}

/* mutator  */
void instrumentr_object_set_data(void* object, SEXP r_data) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    instrumentr_object_remove_data(obj);
    instrumentr_sexp_acquire(r_data);
    obj->r_data = r_data;
}

SEXP r_instrumentr_object_set_data(SEXP r_object, SEXP r_data) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_set_data(object, r_data);
    return R_NilValue;
}

/* mutator  */
void instrumentr_object_remove_data(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    if (instrumentr_object_has_data(obj)) {
        instrumentr_sexp_release(obj->r_data);
        obj->r_data = NULL;
    }
}

SEXP r_instrumentr_object_remove_data(SEXP r_object) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    instrumentr_object_remove_data(object);
    return R_NilValue;
}
