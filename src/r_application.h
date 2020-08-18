#include "Rincludes.h"

extern "C" {
SEXP r_application_create_application(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position);
SEXP r_application_get_name(SEXP r_application);
SEXP r_application_get_directory(SEXP r_application);
SEXP r_application_get_code(SEXP r_application);
SEXP r_application_get_environment(SEXP r_application);
SEXP r_application_get_frame_position(SEXP r_application);
SEXP r_application_get_packages(SEXP r_application);
SEXP r_application_get_call_stack(SEXP r_application);
SEXP r_application_add_package(SEXP r_application, SEXP r_package);
}
