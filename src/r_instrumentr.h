#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_instrumentr_is_tracing_enabled();
SEXP r_instrumentr_disable_tracing();
SEXP r_instrumentr_enable_tracing();
SEXP r_instrumentr_reinstate_tracing();
SEXP r_instrumentr_trace_code(SEXP r_code, SEXP r_environment);
SEXP r_instrumentr_initialize_instrumentr(SEXP r_package_environment,
                                          SEXP r_state_environment,
                                          SEXP r_undefined_object);
SEXP r_instrumentr_finalize_instrumentr();
SEXP r_instrumentr_initialize_tracing();
SEXP r_instrumentr_finalize_tracing();
SEXP r_instrumentr_is_undefined_object(SEXP object);
SEXP r_instrumentr_is_defined_object(SEXP object);

SEXP r_instrumentr_peek_execution_context();
SEXP r_instrumentr_trace_application_load(SEXP r_context, SEXP r_application);
SEXP r_instrumentr_trace_application_unload(SEXP r_context, SEXP r_application);
SEXP r_instrumentr_trace_application_attach(SEXP r_context, SEXP r_application);
SEXP r_instrumentr_trace_application_detach(SEXP r_context, SEXP r_application);
SEXP r_instrumentr_trace_package_load(SEXP r_context,
                                      SEXP r_application,
                                      SEXP r_package);
SEXP r_instrumentr_trace_package_unload(SEXP r_context,
                                        SEXP r_application,
                                        SEXP r_package);
SEXP r_instrumentr_trace_package_attach(SEXP r_context,
                                        SEXP r_application,
                                        SEXP r_package);
SEXP r_instrumentr_trace_package_detach(SEXP r_context,
                                        SEXP r_application,
                                        SEXP r_package);
SEXP r_instrumentr_trace_function_attach(SEXP r_context,
                                         SEXP r_application,
                                         SEXP r_package,
                                         SEXP r_function);
SEXP r_instrumentr_trace_function_detach(SEXP r_context,
                                         SEXP r_application,
                                         SEXP r_package,
                                         SEXP r_function);
SEXP r_instrumentr_trace_call_entry(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function,
                                    SEXP r_call);
SEXP r_instrumentr_trace_call_exit(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package,
                                   SEXP r_function,
                                   SEXP r_result);
SEXP r_instrumentr_trace_call_entry_conditionally(SEXP r_context,
                                                  SEXP r_application,
                                                  SEXP r_package,
                                                  SEXP r_function,
                                                  SEXP r_call_expression,
                                                  SEXP r_call_environment);
SEXP r_instrumentr_trace_call_exit_conditionally(SEXP r_context,
                                                 SEXP r_application,
                                                 SEXP r_package,
                                                 SEXP r_function,
                                                 SEXP r_result);
}
