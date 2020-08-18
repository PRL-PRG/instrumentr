#include "Rincludes.h"

extern "C" {
SEXP r_package_create_package(SEXP r_package_name,
                              SEXP r_package_directory,
                              SEXP r_package_environment);
SEXP r_package_get_name(SEXP r_package);
SEXP r_package_get_directory(SEXP r_package);
SEXP r_package_get_environment(SEXP r_package);
SEXP r_package_get_functions(SEXP r_package);
SEXP r_package_add_function(SEXP r_package, SEXP r_function);
}
