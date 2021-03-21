#ifndef INSTRUMENTR_ALLOC_STATS_H
#define INSTRUMENTR_ALLOC_STATS_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create();

/* mutator */
void instrumentr_alloc_stats_set_model_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type,
    int model_size);

/* mutator */
void instrumentr_alloc_stats_increment_allocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* mutator */
void instrumentr_alloc_stats_increment_deallocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* mutator */
void instrumentr_alloc_stats_increment_finalized_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

/* accessor */
void instrumentr_alloc_stats_increment_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_model_type_t model_type);

INSTRUMENTR_ALLOC_STATS_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ALLOC_STATS_H */
