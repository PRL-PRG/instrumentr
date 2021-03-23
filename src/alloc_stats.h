#ifndef INSTRUMENTR_ALLOC_STATS_H
#define INSTRUMENTR_ALLOC_STATS_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_alloc_stats_t instrumentr_alloc_stats_create();

/* mutator */
void instrumentr_alloc_stats_set_value_size(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type,
    int value_size);

/* mutator */
void instrumentr_alloc_stats_increment_allocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type);

/* mutator */
void instrumentr_alloc_stats_increment_deallocated_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type);

/* mutator */
void instrumentr_alloc_stats_increment_finalized_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type);

/* accessor */
void instrumentr_alloc_stats_increment_zombie_count(
    instrumentr_alloc_stats_t alloc_stats,
    instrumentr_value_type_t value_type);

INSTRUMENTR_ALLOC_STATS_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_ALLOC_STATS_H */
