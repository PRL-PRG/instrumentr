#include "Rincludes.h"

extern "C" {
SEXP r_instrumentr_get_commit_hash();
SEXP r_instrumentr_is_undefined_object(SEXP object);
SEXP r_instrumentr_is_defined_object(SEXP object);
SEXP r_instrumentr_is_tracing_enabled();
SEXP r_instrumentr_enable_tracing();
SEXP r_instrumentr_disable_tracing();
SEXP r_instrumentr_reinstate_tracing();
SEXP r_instrumentr_initialize_instrumentr(SEXP r_package_environment,
                                          SEXP r_state_environment,
                                          SEXP r_undefined_object);
SEXP r_instrumentr_finalize_instrumentr();
SEXP r_instrumentr_get_object_details(SEXP r_value,
                                      SEXP r_variable,
                                      SEXP r_environment,
                                      SEXP r_peek);
}
