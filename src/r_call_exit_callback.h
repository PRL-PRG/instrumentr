#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_call_exit_callback_create_from_r_function(SEXP r_function);
SEXP r_call_exit_callback_create_from_c_function(SEXP r_c_function);
}
