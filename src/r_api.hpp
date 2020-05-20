#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
/* Call */
SEXP r_call_get_id(SEXP r_call);
SEXP r_call_get_package_name(SEXP r_call);
SEXP r_call_get_function_name(SEXP r_call);
SEXP r_call_get_parameter_count(SEXP r_call);
SEXP r_call_get_state(SEXP r_call);
SEXP r_call_get_argument(SEXP r_call, SEXP r_position);

/* Argument */
SEXP r_argument_get_position(SEXP r_argument);
SEXP r_argument_get_name(SEXP r_argument);
SEXP r_argument_is_evaluated(SEXP r_argument);
SEXP r_argument_get_call(SEXP r_argument);

/* CallStack */
SEXP r_call_stack_get_call_stack();
SEXP r_call_stack_get_size(SEXP r_stack);
SEXP r_call_stack_get_frame(SEXP r_stack, SEXP r_index);
SEXP r_call_stack_peek_frame(SEXP r_stack, SEXP r_index);
}
