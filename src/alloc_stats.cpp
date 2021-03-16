#include "alloc_stats.h"
#include "interop.h"
#include "object.h"
#include "model_type.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_alloc_stats_impl_t {
    struct instrumentr_object_impl_t object;

    int model_size[INSTRUMENTR_MODEL_TYPE_COUNT];
    int alive_count[INSTRUMENTR_MODEL_TYPE_COUNT];
    int dead_count[INSTRUMENTR_MODEL_TYPE_COUNT];
    int zombie_count[INSTRUMENTR_MODEL_TYPE_COUNT];
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_alloc_stats_finalize(instrumentr_object_t object) {
    /* NOTE: nothing to do here because fields are statically allocated */
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create() {
    instrumentr_alloc_stats_t alloc_stats =
        (instrumentr_alloc_stats_t) instrumentr_object_create(
            sizeof(instrumentr_alloc_stats_impl_t),
            INSTRUMENTR_OBJECT_TYPE_ALLOC_STATS,
            instrumentr_alloc_stats_finalize);
    return alloc_stats;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_OBJECT_INTEROP_DEFINE_API(alloc_stats,
                                      INSTRUMENTR_OBJECT_TYPE_ALLOC_STATS)

/********************************************************************************
 * object_size
 *******************************************************************************/
/* accessor */
int instrumentr_alloc_stats_get_model_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    return alloc_stats->model_size[(int) (model_type)];
}

/* mutator */
void instrumentr_alloc_stats_set_model_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type,
    int model_size) {
    alloc_stats->model_size[(int) (model_type)] = model_size;
}

/********************************************************************************
 * alive_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    return alloc_stats->alive_count[(int) (model_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    ++alloc_stats->alive_count[(int) (model_type)];
}

/********************************************************************************
 * dead_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    return alloc_stats->dead_count[(int) (model_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    ++alloc_stats->dead_count[(int) (model_type)];
}

/********************************************************************************
 * zombie_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    return alloc_stats->zombie_count[(int) (model_type)];
}

/* accessor */
void instrumentr_alloc_stats_increment_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type) {
    ++alloc_stats->zombie_count[(int) (model_type)];
}

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor */
SEXP instrumentr_alloc_stats_as_data_frame(
    instrumentr_alloc_stats_t alloc_stats) {
    SEXP r_model_type =
        PROTECT(allocVector(STRSXP, INSTRUMENTR_MODEL_TYPE_COUNT));
    SEXP r_model_size =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_MODEL_TYPE_COUNT));
    SEXP r_alive_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_MODEL_TYPE_COUNT));
    SEXP r_dead_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_MODEL_TYPE_COUNT));
    SEXP r_zombie_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_MODEL_TYPE_COUNT));

    for (int i = 0; i < (int) (INSTRUMENTR_MODEL_TYPE_COUNT); ++i) {
        instrumentr_model_type_t model_type = (instrumentr_model_type_t)(i);
        SET_STRING_ELT(r_model_type,
                       i,
                       mkChar(instrumentr_model_type_get_name(model_type)));
        INTEGER(r_model_size)[i] = alloc_stats->model_size[i];
        INTEGER(r_alive_count)[i] = alloc_stats->alive_count[i];
        INTEGER(r_dead_count)[i] = alloc_stats->dead_count[i];
        INTEGER(r_zombie_count)[i] = alloc_stats->zombie_count[i];
    }

    SEXP r_data_frame = PROTECT(instrumentr_create_data_frame(5,
                                                              "model_type",
                                                              r_model_type,
                                                              "model_size",
                                                              r_model_size,
                                                              "alive_count",
                                                              r_alive_count,
                                                              "dead_count",
                                                              r_dead_count,
                                                              "zombie_count",
                                                              r_zombie_count));

    UNPROTECT(6);

    return r_data_frame;
}
