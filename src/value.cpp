#include "value.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "alloc_stats.h"
#include "value_type.h"
#include "model.h"

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_value_finalize(instrumentr_model_t model) {
    instrumentr_value_t value = (instrumentr_value_t) (model);
    /* NOTE: we don't overwrite the type field because it is read by model when
     * freeing the object to update alloc_stats. */
    value->r_sexp = NULL;

    value->finalizer(value);
}

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_value_t
instrumentr_value_create(instrumentr_state_t state,
                         int size,
                         instrumentr_value_type_t type,
                         instrumentr_value_finalizer_t finalizer,
                         instrumentr_origin_t origin,
                         SEXP r_sexp) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 size,
                                 INSTRUMENTR_MODEL_TYPE_VALUE,
                                 instrumentr_value_finalize,
                                 origin);

    instrumentr_value_t value = (instrumentr_value_t) (model);

    value->type = type;
    value->r_sexp = r_sexp;
    value->finalizer = finalizer;

    instrumentr_alloc_stats_t alloc_stats =
        instrumentr_state_get_alloc_stats(state);
    instrumentr_alloc_stats_increment_allocated_count(alloc_stats, value->type);
    instrumentr_alloc_stats_set_value_size(alloc_stats, value->type, size);

    return value;
}

/*******************************************************************************
 * destroy
 *******************************************************************************/

// void instrumentr_value_destroy(instrumentr_value_t value) {
//    /* if value is not killed before */
//    if (value->finalizer != NULL) {
//        instrumentr_value_finalize(value);
//    }
//
//    instrumentr_object_release(value->state);
//    value->state = NULL;
//    free(value);
//}

/********************************************************************************
 * interop
 *******************************************************************************/

void r_instrumentr_value_release(SEXP r_value) {
    r_instrumentr_model_release(r_value);
}

SEXP instrumentr_value_wrap(instrumentr_value_t value) {
    if (value == NULL) {
        instrumentr_log_error("cannot wrap NULL value object");
    }

    SEXP r_value = instrumentr_c_pointer_to_r_externalptr(
        (void*) value, R_NilValue, R_NilValue, r_instrumentr_value_release);
    PROTECT(r_value);
    instrumentr_value_acquire(value);
    instrumentr_sexp_set_class(r_value,
                               instrumentr_value_type_get_class(value->type));
    UNPROTECT(1);
    return r_value;
}

instrumentr_value_t instrumentr_value_unwrap(SEXP r_value,
                                             instrumentr_value_type_t type) {
    instrumentr_value_t value =
        (instrumentr_value_t) instrumentr_r_externalptr_to_c_pointer(r_value);
    if (type == INSTRUMENTR_VALUE_TYPE_COUNT || value->type == type) {
        return value;
    } else {
        instrumentr_log_error(
            "cannot unwrap %d value as %d value", value->type, type);
        /* NOTE: not executed  */
        return NULL;
    }
}

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_value_acquire(instrumentr_value_t value) {
    return instrumentr_model_acquire((instrumentr_model_t) value);
}

/* mutator  */
int instrumentr_value_release(instrumentr_value_t value) {
    return instrumentr_model_release((instrumentr_model_t) value);
}

int instrumentr_value_kill(instrumentr_value_t value) {
    return instrumentr_model_kill((instrumentr_model_t) value);
}

int instrumentr_value_get_reference_count(instrumentr_value_t value) {
    return instrumentr_model_get_reference_count((instrumentr_model_t) value);
}

SEXP r_instrumentr_value_get_reference_count(SEXP r_value) {
    return r_instrumentr_model_get_reference_count(r_value);
}

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_value_get_id(instrumentr_value_t value) {
    return instrumentr_model_get_id((instrumentr_model_t) (value));
}

SEXP r_instrumentr_value_get_id(SEXP r_value) {
    return r_instrumentr_model_get_id(r_value);
}

/*******************************************************************************
 * type
 *******************************************************************************/

instrumentr_value_type_t instrumentr_value_get_type(instrumentr_value_t value) {
    return value->type;
}

/*******************************************************************************
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_value_get_birth_time(instrumentr_value_t value) {
    return instrumentr_model_get_birth_time((instrumentr_model_t) (value));
}

SEXP r_instrumentr_value_get_birth_time(SEXP r_value) {
    return r_instrumentr_model_get_birth_time(r_value);
}

/* accessor */
int instrumentr_value_get_death_time(instrumentr_value_t value) {
    return instrumentr_model_get_death_time((instrumentr_model_t) (value));
}

SEXP r_instrumentr_value_get_death_time(SEXP r_value) {
    return r_instrumentr_model_get_death_time(r_value);
}

/* accessor */
int instrumentr_value_get_life_time(instrumentr_value_t value) {
    return instrumentr_model_get_life_time((instrumentr_model_t) (value));
}

SEXP r_instrumentr_value_get_life_time(SEXP r_value) {
    return r_instrumentr_model_get_life_time(r_value);
}

/*******************************************************************************
 * alive
 *******************************************************************************/

/* accessor */
int instrumentr_value_is_alive(instrumentr_value_t value) {
    return instrumentr_model_is_alive((instrumentr_model_t) value);
}

SEXP r_instrumentr_value_is_alive(SEXP r_value) {
    return r_instrumentr_model_is_alive(r_value);
}

/* accessor */
int instrumentr_value_is_dead(instrumentr_value_t value) {
    return instrumentr_model_is_dead((instrumentr_model_t) value);
}

SEXP r_instrumentr_value_is_dead(SEXP r_value) {
    return r_instrumentr_model_is_dead(r_value);
}

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_value_is_local(instrumentr_value_t value) {
    return instrumentr_model_is_local((instrumentr_model_t) value);
}

SEXP r_instrumentr_value_is_local(SEXP r_value) {
    return r_instrumentr_model_is_local(r_value);
}

/* accessor */
int instrumentr_value_is_foreign(instrumentr_value_t value) {
    return instrumentr_model_is_foreign((instrumentr_model_t) value);
}

SEXP r_instrumentr_value_is_foreign(SEXP r_value) {
    return r_instrumentr_model_is_foreign(r_value);
}

/*******************************************************************************
 * state
 *******************************************************************************/

/* accessor */
instrumentr_state_t instrumentr_value_get_state(instrumentr_value_t value) {
    return instrumentr_model_get_state((instrumentr_model_t) value);
}

/*******************************************************************************
 * r_sexp
 *******************************************************************************/

/* accessor */
SEXP instrumentr_value_get_sexp(instrumentr_value_t value) {
    return value->r_sexp;
}

SEXP r_instrumentr_value_get_sexp(SEXP r_value) {
    instrumentr_value_t value =
        instrumentr_value_unwrap(r_value, INSTRUMENTR_VALUE_TYPE_COUNT);
    return instrumentr_value_get_sexp(value);
}

/*******************************************************************************
 * attribute
 *******************************************************************************/

/* accessor */
instrumentr_value_t instrumentr_value_get_attribute(instrumentr_value_t value,
                                                    const char* name) {
    SEXP r_sexp = instrumentr_value_get_sexp(value);
    SEXP r_name = Rf_install(name);
    SEXP r_attribute = Rf_getAttrib(r_sexp, r_name);
    instrumentr_state_t state = instrumentr_value_get_state(value);
    return instrumentr_state_value_table_lookup(state, r_attribute, 1);
}

SEXP r_instrumentr_value_get_attribute(SEXP r_value, SEXP r_name) {
    instrumentr_value_t value =
        instrumentr_value_unwrap(r_value, INSTRUMENTR_VALUE_TYPE_COUNT);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_value_t result = instrumentr_value_get_attribute(value, name);
    return instrumentr_value_wrap(result);
}
