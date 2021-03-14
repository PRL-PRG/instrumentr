#ifndef INSTRUMENTR_ALLOC_STATS_H
#define INSTRUMENTR_ALLOC_STATS_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create();

/********************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_alloc_stats_destroy(instrumentr_alloc_stats_t alloc_stats);

/********************************************************************************
 * object_size
 *******************************************************************************/
/* accessor */
int instrumentr_alloc_stats_get_object_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/* mutator */
void instrumentr_alloc_stats_set_object_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type,
    int object_size);

/********************************************************************************
 * alive_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/* mutator */
void instrumentr_alloc_stats_increment_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/********************************************************************************
 * dead_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/* mutator */
void instrumentr_alloc_stats_increment_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/********************************************************************************
 * total_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_total_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/* accessor */
void instrumentr_alloc_stats_increment_total_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_object_type_t object_type);

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor */
SEXP instrumentr_alloc_stats_as_data_frame(
    instrumentr_alloc_stats_t alloc_stats);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ALLOC_STATS_H */
