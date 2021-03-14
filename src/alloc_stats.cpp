#include "alloc_stats.h"
#include "interop.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_alloc_stats_impl_t {
    int object_size[INSTRUMENTR_OBJECT];
    int alive_count[INSTRUMENTR_OBJECT];
    int dead_count[INSTRUMENTR_OBJECT];
    int total_count[INSTRUMENTR_OBJECT];
};

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create() {
    instrumentr_alloc_stats_t alloc_stats = (instrumentr_alloc_stats_t)(
        calloc(1, sizeof(struct instrumentr_alloc_stats_impl_t)));
    return alloc_stats;
}

/********************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_alloc_stats_destroy(instrumentr_alloc_stats_t alloc_stats) {
    free(alloc_stats);
}

/********************************************************************************
 * object_size
 *******************************************************************************/
/* accessor */
int instrumentr_alloc_stats_get_object_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    return alloc_stats->object_size[(int) (object_type)];
}

/* mutator */
void instrumentr_alloc_stats_set_object_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type,
    int object_size) {
    alloc_stats->object_size[(int) (object_type)] = object_size;
}

/********************************************************************************
 * alive_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    return alloc_stats->alive_count[(int) (object_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    ++alloc_stats->alive_count[(int) (object_type)];
}

/********************************************************************************
 * dead_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    return alloc_stats->dead_count[(int) (object_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    ++alloc_stats->dead_count[(int) (object_type)];
}

/********************************************************************************
 * total_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_total_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    return alloc_stats->total_count[(int) (object_type)];
}

/* accessor */
void instrumentr_alloc_stats_increment_total_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type) {
    ++alloc_stats->total_count[(int) (object_type)];
}

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor */
SEXP instrumentr_alloc_stats_as_data_frame(
    instrumentr_alloc_stats_t alloc_stats) {
    SEXP r_object_type = PROTECT(allocVector(STRSXP, INSTRUMENTR_OBJECT));
    SEXP r_object_size = PROTECT(allocVector(INTSXP, INSTRUMENTR_OBJECT));
    SEXP r_alive_count = PROTECT(allocVector(INTSXP, INSTRUMENTR_OBJECT));
    SEXP r_dead_count = PROTECT(allocVector(INTSXP, INSTRUMENTR_OBJECT));
    SEXP r_total_count = PROTECT(allocVector(INTSXP, INSTRUMENTR_OBJECT));

    for (int i = 0; i < (int) (INSTRUMENTR_OBJECT); ++i) {
        instrumentr_object_type_t object_type = (instrumentr_object_type_t)(i);
        SET_STRING_ELT(r_object_type,
                       i,
                       mkChar(instrumentr_object_type_to_string(object_type)));
        INTEGER(r_object_size)[i] = alloc_stats->object_size[i];
        INTEGER(r_alive_count)[i] = alloc_stats->alive_count[i];
        INTEGER(r_dead_count)[i] = alloc_stats->dead_count[i];
        INTEGER(r_total_count)[i] = alloc_stats->total_count[i];
    }

    SEXP r_data_frame = PROTECT(instrumentr_create_data_frame(5,
                                                              "object_type",
                                                              r_object_type,
                                                              "object_size",
                                                              r_object_size,
                                                              "alive_count",
                                                              r_alive_count,
                                                              "dead_count",
                                                              r_dead_count,
                                                              "total_count",
                                                              r_total_count));

    UNPROTECT(6);

    return r_data_frame;
}
