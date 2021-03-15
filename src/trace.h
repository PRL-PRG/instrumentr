#ifndef INSTRUMENTR_TRACE_H
#define INSTRUMENTR_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>

SEXP r_instrumentr_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment);

SEXP r_instrumentr_trace_tracing_entry(SEXP r_tracer, SEXP r_application);

SEXP r_instrumentr_trace_tracing_exit(SEXP r_tracer, SEXP r_application);

SEXP r_instrumentr_trace_package_load(SEXP r_tracer,
                                      SEXP r_application,
                                      SEXP r_package);

SEXP r_instrumentr_trace_package_unload(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package);

SEXP r_instrumentr_trace_package_attach(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package);

SEXP r_instrumentr_trace_package_detach(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package);

void instrumentr_trace_builtin_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call);

void instrumentr_trace_builtin_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call);

void instrumentr_trace_special_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call);

void instrumentr_trace_special_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call);

void instrumentr_trace_closure_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call);

void instrumentr_trace_closure_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call);

void instrumentr_trace_context_entry(instrumentr_tracer_t tracer,
                                     instrumentr_application_t application,
                                     instrumentr_context_t context);

void instrumentr_trace_context_exit(instrumentr_tracer_t tracer,
                                    instrumentr_application_t application,
                                    instrumentr_context_t context);

void instrumentr_trace_promise_force_entry(
    instrumentr_tracer_t tracer,
    instrumentr_promise_t promise);

void instrumentr_trace_promise_force_exit(instrumentr_tracer_t tracer,
                                          instrumentr_promise_t promise);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_TRACE_H */
