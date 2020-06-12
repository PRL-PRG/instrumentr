#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_argument_get_name(SEXP r_argument);
SEXP r_argument_is_evaluated(SEXP r_argument);
SEXP r_argument_get_expression(SEXP r_argument);
SEXP r_argument_get_result(SEXP r_argument);
}
