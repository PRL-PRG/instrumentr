#include "exec_stats.h"
#include "interop.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_exec_stats_impl_t {
    clock_t minimum;
    clock_t maximum;
    clock_t total;
    int count;
};

double CLOCKS = CLOCKS_PER_SEC;

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_exec_stats_t instrumentr_exec_stats_create() {
    instrumentr_exec_stats_t exec_stats =
        calloc(1, sizeof(struct instrumentr_exec_stats_impl_t));
    return exec_stats;
}

/********************************************************************************
 * destroy
 *******************************************************************************/

void instrumentr_exec_stats_destroy(instrumentr_exec_stats_t exec_stats) {
    free(exec_stats);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_exec_stats_wrap(instrumentr_exec_stats_t exec_stats) {
    return instrumentr_object_wrap((instrumentr_object_t)(exec_stats));
}

instrumentr_exec_stats_t instrumentr_exec_stats_unwrap(SEXP r_exec_stats) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_exec_stats, INSTRUMENTR_EXEC_STATS);
    return (instrumentr_exec_stats_t)(object);
}

/********************************************************************************
 * minimum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_minimum_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->minimum / CLOCKS;
}

SEXP r_instrumentr_exec_stats_get_minimum_time(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    double result = instrumentr_exec_stats_get_minimum_time(exec_stats);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * maximum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_maximum_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->maximum / CLOCKS;
}

SEXP
r_instrumentr_exec_stats_get_maximum_time(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats = instrumentr_exec_stats_unwrap(r_exec_stats);
    double result = instrumentr_exec_stats_get_maximum_time(exec_stats);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * average
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_average_time(instrumentr_exec_stats_t exec_stats) {
    int count = instrumentr_exec_stats_get_execution_count(exec_stats);
    double total = instrumentr_exec_stats_get_total_time(exec_stats);
    return (count == 0) ? 0 : (total / count);
}

SEXP r_instrumentr_exec_stats_get_average_time(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    double result = instrumentr_exec_stats_get_average_time(exec_stats);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * total
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_total_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->total / CLOCKS;
}

SEXP r_instrumentr_exec_stats_get_total_time(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    double result = instrumentr_exec_stats_get_total_time(exec_stats);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * count
 *******************************************************************************/

/* accessor */
int instrumentr_exec_stats_get_execution_count(
    instrumentr_exec_stats_t exec_stats) {
    return exec_stats->count;
}

SEXP r_instrumentr_exec_stats_get_execution_count(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    int result = instrumentr_exec_stats_get_execution_count(exec_stats);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * data frame
 *******************************************************************************/

/* accessor */
SEXP instrumentr_exec_stats_as_data_frame(instrumentr_exec_stats_t exec_stats) {
    SEXP r_count = PROTECT(instrumentr_c_int_to_r_integer(
        instrumentr_exec_stats_get_execution_count(exec_stats)));
    SEXP r_minimum = PROTECT(instrumentr_c_double_to_r_double(
        instrumentr_exec_stats_get_minimum_time(exec_stats)));
    SEXP r_maximum = PROTECT(instrumentr_c_double_to_r_double(
        instrumentr_exec_stats_get_maximum_time(exec_stats)));
    SEXP r_average = PROTECT(instrumentr_c_double_to_r_double(
        instrumentr_exec_stats_get_average_time(exec_stats)));
    SEXP r_total = PROTECT(instrumentr_c_double_to_r_double(
        instrumentr_exec_stats_get_total_time(exec_stats)));

    SEXP r_data_frame = PROTECT(instrumentr_create_data_frame(5,
                                                              "execution_count",
                                                              r_count,
                                                              "minimum_time",
                                                              r_minimum,
                                                              "maximum_time",
                                                              r_maximum,
                                                              "average_time",
                                                              r_average,
                                                              "total_time",
                                                              r_total));

    UNPROTECT(6);

    return r_data_frame;
}

SEXP r_instrumentr_exec_stats_as_data_frame(SEXP r_exec_stats) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    SEXP r_data_frame = instrumentr_exec_stats_as_data_frame(exec_stats);
    return r_data_frame;
}


/********************************************************************************
 * minimum + maximum + average + total + count
 *******************************************************************************/

/* mutator */
void instrumentr_exec_stats_update(instrumentr_exec_stats_t exec_stats,
                                   clock_t time) {
    ++exec_stats->count;
    exec_stats->total += time;
    if (time > exec_stats->maximum) {
        exec_stats->maximum = time;
    }
    /* set the first time to the minimum value */
    if (exec_stats->count == 1 || time < exec_stats->minimum) {
        exec_stats->minimum = time;
    }
}
