#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_call_stack_get_size(SEXP r_call_stack);
SEXP r_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index);
}
