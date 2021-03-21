#ifndef INSTRUMENTR_EXEC_STATS_H
#define INSTRUMENTR_EXEC_STATS_H

#include <instrumentr/api.h>
#include "model.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_exec_stats_t instrumentr_exec_stats_create();

/********************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_exec_stats_destroy(instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_exec_stats_wrap(instrumentr_exec_stats_t exec_stats);

instrumentr_exec_stats_t instrumentr_exec_stats_unwrap(SEXP r_exec_stats);

void instrumentr_exec_stats_update(instrumentr_exec_stats_t exec_stats,
                                   instrumentr_event_t event,
                                   clock_t time);

INSTRUMENTR_EXEC_STATS_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EXEC_STATS_H */
