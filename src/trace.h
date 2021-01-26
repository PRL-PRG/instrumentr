#ifndef INSTRUMENTR_TRACE_H
#define INSTRUMENTR_TRACE_H

#include <instrumentr/Rincludes.h>

SEXP r_instrumentr_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment);

SEXP r_instrumentr_trace_tracing_initialization(SEXP r_tracer, SEXP r_application);

SEXP r_instrumentr_trace_tracing_finalization(SEXP r_tracer, SEXP r_application);

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

#endif /* INSTRUMENTR_TRACE_H */
