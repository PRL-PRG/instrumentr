#include "Rincludes.h"

extern "C" {
SEXP r_parameter_get_name(SEXP r_parameter);
SEXP r_parameter_get_position(SEXP r_parameter);
SEXP r_parameter_is_missing(SEXP r_parameter);
SEXP r_parameter_is_vararg(SEXP r_parameter);
SEXP r_parameter_get_arguments(SEXP r_parameter);
SEXP r_parameter_get_argument_by_name(SEXP r_parameter, SEXP r_name);
SEXP r_parameter_get_argument_by_position(SEXP r_parameter, SEXP r_position);
}
