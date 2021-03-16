#ifndef INSTRUMENTR_ALLOC_STATS_H
#define INSTRUMENTR_ALLOC_STATS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create();

/********************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_alloc_stats_destroy(instrumentr_alloc_stats_t alloc_stats);

/********************************************************************************
 * model_size
 *******************************************************************************/
/* accessor */
int instrumentr_alloc_stats_get_model_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* mutator */
void instrumentr_alloc_stats_set_model_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type,
    int model_size);

/********************************************************************************
 * alive_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* mutator */
void instrumentr_alloc_stats_increment_alive_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/********************************************************************************
 * dead_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* mutator */
void instrumentr_alloc_stats_increment_dead_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/********************************************************************************
 * zombie_count
 *******************************************************************************/

/* accessor */
int instrumentr_alloc_stats_get_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* accessor */
void instrumentr_alloc_stats_increment_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

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
