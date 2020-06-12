#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_function_create_function(SEXP r_function_name,
                                SEXP r_parameter_count,
                                SEXP r_function_object);
SEXP r_function_get_name(SEXP r_function);
SEXP r_function_get_parameter_count(SEXP r_function);
SEXP r_function_get_definition(SEXP r_function);
}
