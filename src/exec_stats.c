#include "exec_stats.h"
#include "interop.h"
#include <limits.h>

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
    exec_stats->minimum = LONG_MAX;
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
    SEXP r_exec_stats = PROTECT(allocVector(VECSXP, 5));
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

    SET_VECTOR_ELT(r_exec_stats, 0, r_count);
    SET_VECTOR_ELT(r_exec_stats, 1, r_minimum);
    SET_VECTOR_ELT(r_exec_stats, 2, r_maximum);
    SET_VECTOR_ELT(r_exec_stats, 3, r_average);
    SET_VECTOR_ELT(r_exec_stats, 4, r_total);

    SEXP r_column_names = PROTECT(allocVector(STRSXP, 5));
    SET_STRING_ELT(r_column_names, 0, mkChar("execution_count"));
    SET_STRING_ELT(r_column_names, 1, mkChar("minimum_time"));
    SET_STRING_ELT(r_column_names, 2, mkChar("maximum_time"));
    SET_STRING_ELT(r_column_names, 3, mkChar("average_time"));
    SET_STRING_ELT(r_column_names, 4, mkChar("total_time"));

    setAttrib(r_exec_stats, R_NamesSymbol, r_column_names);

    SEXP r_row_names = PROTECT(allocVector(STRSXP, 1));

    SET_STRING_ELT(r_row_names, 0, mkChar("1"));

    setAttrib(r_exec_stats, R_RowNamesSymbol, r_row_names);

    instrumentr_sexp_set_class(r_exec_stats, mkString("data.frame"));

    UNPROTECT(8);

    return r_exec_stats;
}

/********************************************************************************
 * minimum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_minimum_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->minimum / CLOCKS;
}

/********************************************************************************
 * maximum
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_maximum_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->maximum / CLOCKS;
}

/********************************************************************************
 * average
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_average_time(instrumentr_exec_stats_t exec_stats) {
    return instrumentr_exec_stats_get_total_time(exec_stats) /
           instrumentr_exec_stats_get_execution_count(exec_stats);
}

/********************************************************************************
 * total
 *******************************************************************************/

/* accessor */
double
instrumentr_exec_stats_get_total_time(instrumentr_exec_stats_t exec_stats) {
    return exec_stats->total / CLOCKS;
}

/********************************************************************************
 * count
 *******************************************************************************/

/* accessor */
int instrumentr_exec_stats_get_execution_count(
    instrumentr_exec_stats_t exec_stats) {
    return exec_stats->count;
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
    if (time < exec_stats->minimum) {
        exec_stats->minimum = time;
    }
}
