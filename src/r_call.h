#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_call_create_call(SEXP r_function,
                        SEXP r_call_expression,
                        SEXP r_environment,
                        SEXP r_frame_position);
SEXP r_call_get_function(SEXP r_call);
SEXP r_call_get_expression(SEXP r_call);
SEXP r_call_get_environment(SEXP r_call);
SEXP r_call_get_frame_position(SEXP r_call);
SEXP r_call_is_active(SEXP r_call);
SEXP r_call_is_successful(SEXP r_call);
SEXP r_call_get_result(SEXP r_call);
SEXP r_call_get_parameters(SEXP r_call);
SEXP r_call_get_parameter_by_name(SEXP r_call, SEXP r_name);
SEXP r_call_get_parameter_by_position(SEXP r_call, SEXP r_position);
}
