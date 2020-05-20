#define USE_RINTERNALS
#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
/* Call */
SEXP r_get_id(SEXP r_call);
SEXP r_get_package_name(SEXP r_call);
SEXP r_get_function_name(SEXP r_call);
SEXP r_get_parameter_count(SEXP r_call);
SEXP r_get_state(SEXP r_call);

/* Argument */
SEXP r_get_position(SEXP r_argument);
SEXP r_get_name(SEXP r_argument);
SEXP r_is_evaluated(SEXP r_argument);
SEXP r_get_call(SEXP r_argument);
}
