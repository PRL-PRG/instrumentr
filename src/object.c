#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "alloc_stats.h"

/*******************************************************************************
 * to_string
 *******************************************************************************/

const char* instrumentr_object_type_to_string(instrumentr_object_type_t object_type) {
    switch(object_type) {
    case INSTRUMENTR_TRACER: return "tracer"; break;
    case INSTRUMENTR_CALLBACK: return "callback"; break;
    case INSTRUMENTR_STATE: return "state"; break;
    case INSTRUMENTR_APPLICATION: return "application"; break;
    case INSTRUMENTR_PACKAGE: return "package"; break;
    case INSTRUMENTR_FUNCTION: return "function"; break;
    case INSTRUMENTR_CALL: return "call"; break;
    case INSTRUMENTR_CALL_STACK: return "call_stack"; break;
    case INSTRUMENTR_PARAMETER: return "parameter"; break;
    case INSTRUMENTR_ARGUMENT: return "argument"; break;
    case INSTRUMENTR_PROMISE: return "promise"; break;
    case INSTRUMENTR_VALUE: return "value"; break;
    case INSTRUMENTR_FRAME: return "frame"; break;
    case INSTRUMENTR_CONTEXT: return "context"; break;
    case INSTRUMENTR_EXEC_STATS: return "exec_stats"; break;
    case INSTRUMENTR_ALLOC_STATS: return "alloc_stats"; break;
    case INSTRUMENTR_OBJECT: return "object"; break;
    }
}



/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size) {
    instrumentr_object_t object = (instrumentr_object_t) calloc(1, size);
    if (object == NULL) {
        Rf_error("allocation of new instrumentr object failed");
    }
    return object;
}

void
instrumentr_object_initialize(instrumentr_object_t object,
                              instrumentr_state_t state,
                              instrumentr_object_type_t type,
                              instrumentr_object_finalizer_t finalizer,
                              instrumentr_origin_t origin) {
    object->id = instrumentr_state_get_next_id(state);
    object->type = type;
    object->reference_count = 1;
    object->finalizer = finalizer;
    object->birth_time = instrumentr_state_get_time(state);
    object->death_time = -1;
    object->origin = origin;
    object->state = state;
    instrumentr_object_acquire(state);
}


instrumentr_object_t
instrumentr_object_create_and_initialize(int size,
                                         instrumentr_state_t state,
                                         instrumentr_object_type_t type,
                                         instrumentr_object_finalizer_t finalizer,
                                         instrumentr_origin_t origin) {

    instrumentr_object_t object = instrumentr_object_create(size);
    instrumentr_object_initialize(object, state, type, finalizer, origin);

    instrumentr_alloc_stats_t alloc_stats = instrumentr_state_get_alloc_stats(state);
    instrumentr_alloc_stats_increment_alive_count(alloc_stats, object -> type);
    instrumentr_alloc_stats_set_object_size(alloc_stats, object -> type, size);

    return object;
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_finalize(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);

    if(obj->finalizer == NULL) {
        Rf_error("attempt to finalize already finalized object");
    }
    obj->finalizer(obj);
    obj->finalizer = NULL;
    /* this is the point where the object's kernel is deleted */
    obj->death_time = instrumentr_state_get_time(obj -> state);
}


/*******************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_object_destroy(instrumentr_object_t object) {
    /* if object is not killed before */
    if(object -> finalizer != NULL) {
        instrumentr_object_finalize(object);
    }

    instrumentr_alloc_stats_t alloc_stats = instrumentr_state_get_alloc_stats(object->state);
    instrumentr_alloc_stats_increment_dead_count(alloc_stats, object -> type);
    instrumentr_object_release(object->state);
    object->state = NULL;
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

void instrumentr_object_kill(void* object) {
    int ref_count = instrumentr_object_release(object);

    /* this means the object is not destroyed  */
    if(ref_count != 0) {
        instrumentr_object_t obj = (instrumentr_object_t)(object);

        instrumentr_object_finalize(object);

        instrumentr_alloc_stats_t alloc_stats = instrumentr_state_get_alloc_stats(obj -> state);
        instrumentr_alloc_stats_increment_zombie_count(alloc_stats, obj -> type);
    }
}


int instrumentr_object_get_ref_count(void* object) {
    instrumentr_object_t obj = (instrumentr_object_t)(object);
    return obj -> reference_count;
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

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_object_is_local(instrumentr_object_t object) {
    return object -> origin == INSTRUMENTR_ORIGIN_LOCAL;
}

SEXP r_instrumentr_object_is_local(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_local(object);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
int instrumentr_object_is_foreign(instrumentr_object_t object) {
    return object -> origin == INSTRUMENTR_ORIGIN_FOREIGN;
}

SEXP r_instrumentr_object_is_foreign(SEXP r_object) {
    instrumentr_object_t object = instrumentr_object_unwrap(r_object, INSTRUMENTR_OBJECT);
    int result = instrumentr_object_is_foreign(object);
    return instrumentr_c_int_to_r_logical(result);
}
