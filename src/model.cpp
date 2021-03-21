#include "model.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "alloc_stats.h"
#include "model_type.h"

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_model_t
instrumentr_model_create(instrumentr_state_t state,
                         int size,
                         instrumentr_model_type_t type,
                         instrumentr_model_finalizer_t finalizer,
                         instrumentr_origin_t origin) {
    instrumentr_model_t model = (instrumentr_model_t) calloc(1, size);
    if (model == NULL) {
        Rf_error("allocation of new instrumentr model failed");
    }

    model->id = instrumentr_state_get_next_id(state);
    model->type = type;
    model->reference_count = 1;
    model->finalizer = finalizer;
    model->birth_time = instrumentr_state_get_time(state);
    model->death_time = -1;
    model->origin = origin;
    model->state = state;
    instrumentr_object_acquire(state);

    instrumentr_alloc_stats_t alloc_stats =
        instrumentr_state_get_alloc_stats(state);
    instrumentr_alloc_stats_increment_allocated_count(alloc_stats, model->type);
    instrumentr_alloc_stats_set_model_size(alloc_stats, model->type, size);

    return model;
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_model_finalize(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);

    if (mod->finalizer == NULL) {
        Rf_error("attempt to finalize already finalized model");
    }
    mod->finalizer(mod);
    mod->finalizer = NULL;
    /* this is the point where the model's kernel is deleted */
    mod->death_time = instrumentr_state_get_time(mod->state);

    instrumentr_alloc_stats_t alloc_stats =
        instrumentr_state_get_alloc_stats(mod->state);
    instrumentr_alloc_stats_increment_finalized_count(alloc_stats, mod->type);
}

/*******************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_model_destroy(instrumentr_model_t model) {
    /* if model is not killed before */
    if (model->finalizer != NULL) {
        instrumentr_model_finalize(model);
    }

    instrumentr_alloc_stats_t alloc_stats =
        instrumentr_state_get_alloc_stats(model->state);
    instrumentr_alloc_stats_increment_deallocated_count(alloc_stats,
                                                        model->type);
    instrumentr_object_release(model->state);
    model->state = NULL;
    free(model);
}

/********************************************************************************
 * interop
 *******************************************************************************/

void r_instrumentr_model_release(SEXP r_model) {
    void* model = instrumentr_r_externalptr_to_c_pointer(r_model);
    if (model == NULL) {
        instrumentr_log_error("instrumentr model finalizer encountered NULL "
                              "model in externalptr");
    }
    R_ClearExternalPtr(r_model);
    instrumentr_model_release(model);
}

SEXP instrumentr_model_wrap(instrumentr_model_t model) {
    if (model == NULL) {
        instrumentr_log_error("cannot wrap NULL model object");
    }

    SEXP r_model = instrumentr_c_pointer_to_r_externalptr(
        model, R_NilValue, R_NilValue, r_instrumentr_model_release);
    PROTECT(r_model);
    instrumentr_model_acquire(model);
    instrumentr_sexp_set_class(r_model,
                               instrumentr_model_type_get_class(model->type));
    UNPROTECT(1);
    return r_model;
}

instrumentr_model_t instrumentr_model_unwrap(SEXP r_model,
                                             instrumentr_model_type_t type) {
    instrumentr_model_t model =
        (instrumentr_model_t) instrumentr_r_externalptr_to_c_pointer(r_model);
    if (type == INSTRUMENTR_MODEL_TYPE_COUNT || model->type == type) {
        return model;
    } else {
        instrumentr_log_error(
            "cannot unwrap %d model as %d model", model->type, type);
        /* NOTE: not executed  */
        return NULL;
    }
}

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_model_acquire(void* model) {
    instrumentr_model_t obj = (instrumentr_model_t)(model);
    obj->reference_count += 1;
    return obj->reference_count;
}

/* mutator  */
int instrumentr_model_release(void* model) {
    instrumentr_model_t obj = (instrumentr_model_t)(model);
    obj->reference_count -= 1;
    int reference_count = obj->reference_count;

    if (reference_count == 0) {
        instrumentr_model_destroy(obj);
    }

    return reference_count;
}

int instrumentr_model_kill(void* model) {
    int reference_count = instrumentr_model_release(model);

    /* this means the model is not destroyed  */
    if (reference_count != 0) {
        instrumentr_model_finalize(model);

        instrumentr_model_t mod = (instrumentr_model_t)(model);
        instrumentr_alloc_stats_t alloc_stats =
            instrumentr_state_get_alloc_stats(mod->state);
        instrumentr_alloc_stats_increment_zombie_count(alloc_stats, mod->type);
    }

    return reference_count;
}

int instrumentr_model_get_reference_count(void* model) {
    instrumentr_model_t obj = (instrumentr_model_t)(model);
    return obj->reference_count;
}

SEXP r_instrumentr_model_get_reference_count(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int reference_count = instrumentr_model_get_reference_count(model);
    return instrumentr_c_int_to_r_integer(reference_count);
}

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_model_get_id(void* model) {
    instrumentr_model_t obj = (instrumentr_model_t)(model);
    return obj->id;
}

SEXP r_instrumentr_model_get_id(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    instrumentr_id_t id = instrumentr_model_get_id(model);
    return instrumentr_c_int_to_r_integer(id);
}

/*******************************************************************************
 * type
 *******************************************************************************/

instrumentr_model_type_t instrumentr_model_get_type(void* model) {
    instrumentr_model_t obj = (instrumentr_model_t)(model);
    return obj->type;
}

/*******************************************************************************
 * time
 *******************************************************************************/

/* accessor */
int instrumentr_model_get_birth_time(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    return mod->birth_time;
}

SEXP r_instrumentr_model_get_birth_time(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_get_birth_time(model);
    return instrumentr_c_int_to_r_integer(result);
}

/* accessor */
int instrumentr_model_get_death_time(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    return mod->death_time;
}

SEXP r_instrumentr_model_get_death_time(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_get_death_time(model);
    return instrumentr_c_int_to_r_integer(result);
}

/* accessor */
int instrumentr_model_get_life_time(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    if (mod->death_time < 0) {
        return mod->death_time;
    } else if (mod->birth_time < 0) {
        return mod->birth_time;
    } else {
        return mod->death_time - mod->birth_time;
    }
}

SEXP r_instrumentr_model_get_life_time(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_get_life_time(model);
    return instrumentr_c_int_to_r_integer(result);
}

/*******************************************************************************
 * alive
 *******************************************************************************/

/* accessor */
int instrumentr_model_is_alive(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    return !(mod->death_time > 0);
}

SEXP r_instrumentr_model_is_alive(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_is_alive(model);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
int instrumentr_model_is_dead(void* model) {
    return !instrumentr_model_is_alive(model);
}

SEXP r_instrumentr_model_is_dead(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_is_dead(model);
    return instrumentr_c_int_to_r_logical(result);
}

/*******************************************************************************
 * origin
 *******************************************************************************/

/* accessor */
int instrumentr_model_is_local(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    return mod->origin == INSTRUMENTR_ORIGIN_LOCAL;
}

SEXP r_instrumentr_model_is_local(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_is_local(model);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor */
int instrumentr_model_is_foreign(void* model) {
    instrumentr_model_t mod = (instrumentr_model_t)(model);
    return mod->origin == INSTRUMENTR_ORIGIN_FOREIGN;
}

SEXP r_instrumentr_model_is_foreign(SEXP r_model) {
    instrumentr_model_t model =
        instrumentr_model_unwrap(r_model, INSTRUMENTR_MODEL_TYPE_COUNT);
    int result = instrumentr_model_is_foreign(model);
    return instrumentr_c_int_to_r_logical(result);
}
