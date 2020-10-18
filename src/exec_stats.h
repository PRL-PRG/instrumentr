#ifndef INSTRUMENTR_EXEC_STATS_H
#define INSTRUMENTR_EXEC_STATS_H

#include <time.h>
#include <instrumentr/Rincludes.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_exec_stats_impl_t* instrumentr_exec_stats_t;

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
instrumentr_exec_stats_get_minimum_time(instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * maximum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_maximum_time(instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * average
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_average_time(instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * total
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_total_time(instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * count
 *******************************************************************************/

/* accessor */
int instrumentr_exec_stats_get_execution_count(
    instrumentr_exec_stats_t exec_stats);

/********************************************************************************
 * minimum + maximum + average + total + count
 *******************************************************************************/

/* mutator */
void instrumentr_exec_stats_update(instrumentr_exec_stats_t exec_stats, clock_t time);

#endif /* INSTRUMENTR_EXEC_STATS_H */
