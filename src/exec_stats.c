#include "exec_stats.h"
#include "interop.h"
#include "object.h"
#include "event.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_exec_stats_impl_t {
    clock_t minimum[INSTRUMENTR_EVENT_COUNT + 1];
    clock_t maximum[INSTRUMENTR_EVENT_COUNT + 1];
    clock_t total[INSTRUMENTR_EVENT_COUNT + 1];
    int count[INSTRUMENTR_EVENT_COUNT + 1];
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
instrumentr_exec_stats_get_minimum_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event) {
    return exec_stats->minimum[(int)event] / CLOCKS;
}

SEXP r_instrumentr_exec_stats_get_minimum_time(SEXP r_exec_stats,
                                               SEXP r_event) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    double result = instrumentr_exec_stats_get_minimum_time(exec_stats, event);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * maximum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_maximum_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event) {
    return exec_stats->maximum[(int)event] / CLOCKS;
}

SEXP
r_instrumentr_exec_stats_get_maximum_time(SEXP r_exec_stats,
                                          SEXP r_event) {
    instrumentr_exec_stats_t exec_stats = instrumentr_exec_stats_unwrap(r_exec_stats);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    double result = instrumentr_exec_stats_get_maximum_time(exec_stats, event);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * average
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_average_time(instrumentr_exec_stats_t exec_stats,
                                        instrumentr_event_t event) {
    int count = instrumentr_exec_stats_get_execution_count(exec_stats, event);
    double total = instrumentr_exec_stats_get_total_time(exec_stats, event);
    return (count == 0) ? 0 : (total / count);
}

SEXP r_instrumentr_exec_stats_get_average_time(SEXP r_exec_stats,
                                               SEXP r_event) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    double result = instrumentr_exec_stats_get_average_time(exec_stats, event);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * total
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_total_time(instrumentr_exec_stats_t exec_stats,
                                      instrumentr_event_t event) {
    return exec_stats->total[(int)event] / CLOCKS;
}

SEXP r_instrumentr_exec_stats_get_total_time(SEXP r_exec_stats,
                                             SEXP r_event) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    double result = instrumentr_exec_stats_get_total_time(exec_stats, event);
    return instrumentr_c_double_to_r_double(result);
}

/********************************************************************************
 * count
 *******************************************************************************/

/* accessor */
int instrumentr_exec_stats_get_execution_count(
                                               instrumentr_exec_stats_t exec_stats,
                                               instrumentr_event_t event) {
    return exec_stats->count[(int)event];
}

SEXP r_instrumentr_exec_stats_get_execution_count(SEXP r_exec_stats,
                                                  SEXP r_event) {
    instrumentr_exec_stats_t exec_stats =
        instrumentr_exec_stats_unwrap(r_exec_stats);
    instrumentr_event_t event = instrumentr_event_unwrap(r_event);
    int result = instrumentr_exec_stats_get_execution_count(exec_stats, event);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * minimum + maximum + average + total + count
 *******************************************************************************/

/* mutator */
void instrumentr_exec_stats_update(instrumentr_exec_stats_t exec_stats,
                                   instrumentr_event_t event,
                                   clock_t time) {
    int index = (int)event;

    ++exec_stats->count[index];
    exec_stats->total[index] += time;
    if (time > exec_stats->maximum[index]) {
        exec_stats->maximum[index] = time;
    }
    /* set the first time to the minimum value */
    if (exec_stats->count[index] == 1 || time < exec_stats->minimum[index]) {
        exec_stats->minimum[index] = time;
    }
}

/********************************************************************************
 * data_frame
 *******************************************************************************/

void assign_exec_stats_fields(instrumentr_exec_stats_t exec_stats,
                              instrumentr_event_t event,
                              const char* event_name,
                              int index,
                              SEXP r_callback,
                              SEXP r_count,
                              SEXP r_minimum,
                              SEXP r_maximum,
                              SEXP r_average,
                              SEXP r_total) {
    SET_STRING_ELT(r_callback, index, mkChar(event_name));

    int count = instrumentr_exec_stats_get_execution_count(exec_stats, event);
    INTEGER(r_count)[index] = count;

    double minimum = instrumentr_exec_stats_get_minimum_time(exec_stats, event);
    REAL(r_minimum)[index] = minimum;

    double maximum = instrumentr_exec_stats_get_maximum_time(exec_stats, event);
    REAL(r_maximum)[index] = maximum;

    double average = instrumentr_exec_stats_get_average_time(exec_stats, event);
    REAL(r_average)[index] = average;

    double total = instrumentr_exec_stats_get_total_time(exec_stats, event);
    REAL(r_total)[index] = total;
}

SEXP instrumentr_exec_stats_as_data_frame(instrumentr_exec_stats_t exec_stats) {

    int row_count = INSTRUMENTR_EVENT_COUNT + 1;

    SEXP r_event = PROTECT(allocVector(STRSXP, row_count));
    SEXP r_count = PROTECT(allocVector(INTSXP, row_count));
    SEXP r_minimum = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_maximum = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_average = PROTECT(allocVector(REALSXP, row_count));
    SEXP r_total = PROTECT(allocVector(REALSXP, row_count));

    int index;

    for (index = 0; index < INSTRUMENTR_EVENT_COUNT; ++index) {
        instrumentr_event_t event = (instrumentr_event_t)index;

        assign_exec_stats_fields(
            exec_stats,
            event,
            instrumentr_event_to_string(event),
            index,
            r_event,
            r_count,
            r_minimum,
            r_maximum,
            r_average,
            r_total);
    }

    assign_exec_stats_fields(exec_stats,
                             INSTRUMENTR_EVENT_COUNT,
                             "tracing",
                             index,
                             r_event,
                             r_count,
                             r_minimum,
                             r_maximum,
                             r_average,
                             r_total);

    SEXP r_data_frame = PROTECT(instrumentr_create_data_frame(6,
                                                              "exec",
                                                              r_event,
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

    UNPROTECT(7);

    return r_data_frame;
}
