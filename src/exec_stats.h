#ifndef INSTRUMENTR_EXEC_STATS_H
#define INSTRUMENTR_EXEC_STATS_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
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

/********************************************************************************
 * minimum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_minimum_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event);
SEXP r_instrumentr_exec_stats_get_minimum_time(SEXP r_exec_stats, SEXP r_event);

/********************************************************************************
 * maximum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_maximum_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event);
SEXP r_instrumentr_exec_stats_get_maximum_time(SEXP r_exec_stats, SEXP r_event);

/********************************************************************************
 * average
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_average_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event);
SEXP r_instrumentr_exec_stats_get_average_time(SEXP r_exec_stats, SEXP r_event);

/********************************************************************************
 * total
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_total_time(instrumentr_exec_stats_t exec_stats,
                                      instrumentr_event_t event);
SEXP r_instrumentr_exec_stats_get_total_time(SEXP r_exec_stats, SEXP r_event);

/********************************************************************************
 * count
 *******************************************************************************/

/* accessor */
int instrumentr_exec_stats_get_execution_count(
    instrumentr_exec_stats_t exec_stats,
    instrumentr_event_t event);
SEXP r_instrumentr_exec_stats_get_execution_count(SEXP r_exec_stats,
                                                  SEXP r_event);

/********************************************************************************
 * minimum + maximum + average + total + count
 *******************************************************************************/

/* mutator */
void instrumentr_exec_stats_update(instrumentr_exec_stats_t exec_stats,
                                   instrumentr_event_t event,
                                   clock_t time);

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_exec_stats_as_data_frame(instrumentr_exec_stats_t exec_stats);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EXEC_STATS_H */
