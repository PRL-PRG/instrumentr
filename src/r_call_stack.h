#include "Rincludes.h"

extern "C" {
SEXP r_call_stack_get_size(SEXP r_call_stack);
SEXP r_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index);
}
