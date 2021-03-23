#include "alloc_stats.h"
#include "interop.h"
#include "object.h"
#include "value_type.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_alloc_stats_impl_t {
    struct instrumentr_object_impl_t object;

    int value_size[INSTRUMENTR_VALUE_TYPE_COUNT];
    int allocated_count[INSTRUMENTR_VALUE_TYPE_COUNT];
    int deallocated_count[INSTRUMENTR_VALUE_TYPE_COUNT];
    int finalized_count[INSTRUMENTR_VALUE_TYPE_COUNT];
    int max_alive_count[INSTRUMENTR_VALUE_TYPE_COUNT];
    int zombie_count[INSTRUMENTR_VALUE_TYPE_COUNT];
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
int instrumentr_alloc_stats_get_value_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->value_size[(int) (value_type)];
}

/* mutator */
void instrumentr_alloc_stats_set_value_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type,
    int value_size) {
    alloc_stats->value_size[(int) (value_type)] = value_size;
}

/********************************************************************************
 * allocated_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_allocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->allocated_count[(int) (value_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_allocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    int upper = ++alloc_stats->allocated_count[(int) (value_type)];
    int lower = alloc_stats->deallocated_count[(int) (value_type)];
    int current_max = upper - lower;
    int previous_max = alloc_stats->max_alive_count[(int) (value_type)];

    if (current_max > previous_max) {
        alloc_stats->max_alive_count[(int) (value_type)] = current_max;
    }
}

/********************************************************************************
 * deallocated_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_deallocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->deallocated_count[(int) (value_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_deallocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    ++alloc_stats->deallocated_count[(int) (value_type)];
}

/********************************************************************************
 * finalized_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_finalized_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->finalized_count[(int) (value_type)];
}

/* mutator */
void instrumentr_alloc_stats_increment_finalized_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    ++alloc_stats->finalized_count[(int) (value_type)];
}

/********************************************************************************
 * zombie_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->zombie_count[(int) (value_type)];
}

/* accessor */
void instrumentr_alloc_stats_increment_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    ++alloc_stats->zombie_count[(int) (value_type)];
}

/********************************************************************************
 * max_alive_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_max_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type) {
    return alloc_stats->max_alive_count[(int) (value_type)];
}

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor */
SEXP instrumentr_alloc_stats_as_data_frame(
    instrumentr_alloc_stats_t alloc_stats) {
    SEXP r_value_type =
        PROTECT(allocVector(STRSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_value_size =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_allocated_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_deallocated_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_finalized_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_max_alive_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));
    SEXP r_zombie_count =
        PROTECT(allocVector(INTSXP, INSTRUMENTR_VALUE_TYPE_COUNT));

    for (int i = 0; i < (int) (INSTRUMENTR_VALUE_TYPE_COUNT); ++i) {
        instrumentr_value_type_t value_type = (instrumentr_value_type_t)(i);
        SET_STRING_ELT(r_value_type,
                       i,
                       mkChar(instrumentr_value_type_get_name(value_type)));
        INTEGER(r_value_size)[i] = alloc_stats->value_size[i];
        INTEGER(r_allocated_count)[i] = alloc_stats->allocated_count[i];
        INTEGER(r_deallocated_count)[i] = alloc_stats->deallocated_count[i];
        INTEGER(r_finalized_count)[i] = alloc_stats->finalized_count[i];
        INTEGER(r_max_alive_count)[i] = alloc_stats->max_alive_count[i];
        INTEGER(r_zombie_count)[i] = alloc_stats->zombie_count[i];
    }

    SEXP r_data_frame =
        PROTECT(instrumentr_create_data_frame(7,
                                              "type",
                                              r_value_type,
                                              "size",
                                              r_value_size,
                                              "allocated",
                                              r_allocated_count,
                                              "deallocated",
                                              r_deallocated_count,
                                              "finalized",
                                              r_finalized_count,
                                              "max_alive",
                                              r_max_alive_count,
                                              "zombie",
                                              r_zombie_count));

    UNPROTECT(8);

    return r_data_frame;
}
