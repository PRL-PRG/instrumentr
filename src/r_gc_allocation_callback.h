#include "Rincludes.h"

extern "C" {
SEXP r_gc_allocation_callback_create_from_r_function(SEXP r_function);
SEXP r_gc_allocation_callback_create_from_c_function(SEXP r_c_function);
}
