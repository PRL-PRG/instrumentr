#ifndef INSTRUMENTR_TRACE_H
#define INSTRUMENTR_TRACE_H

#include <instrumentr/Rincludes.h>

#ifdef __cplusplus
extern "C" {
#endif

SEXP r_instrumentr_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment);

SEXP r_instrumentr_trace_tracing_entry(SEXP r_tracer,
                                       SEXP r_workdir,
                                       SEXP r_code,
                                       SEXP r_appenv);

SEXP r_instrumentr_trace_tracing_exit(SEXP r_tracer);

SEXP r_instrumentr_trace_package_load(SEXP r_tracer, SEXP r_package_name);

SEXP r_instrumentr_trace_package_unload(SEXP r_tracer, SEXP r_package_name);

SEXP r_instrumentr_trace_package_attach(SEXP r_tracer, SEXP r_package_name);

SEXP r_instrumentr_trace_package_detach(SEXP r_tracer, SEXP r_package_name);

void instrumentr_trace_call_entry(dyntracer_t* dyntracer,
                                  SEXP r_call,
                                  SEXP r_op,
                                  SEXP r_args,
                                  SEXP r_rho,
                                  dyntrace_dispatch_t dispatch);

void instrumentr_trace_call_exit(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch,
                                 SEXP r_result);

void instrumentr_trace_context_entry(dyntracer_t* dyntracer, void* pointer);

void instrumentr_trace_context_exit(dyntracer_t* dyntracer, void* pointer);

void instrumentr_trace_context_jump(dyntracer_t* dyntracer,
                                    void* context,
                                    const SEXP return_value,
                                    int restart);

void instrumentr_trace_eval_entry(dyntracer_t* dyntracer,
                                  SEXP r_expression,
                                  SEXP r_rho);

void instrumentr_trace_eval_exit(dyntracer_t* dyntracer,
                                 SEXP r_expression,
                                 SEXP r_rho,
                                 SEXP r_result);

void instrumentr_trace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object);

void instrumentr_trace_gc_deallocation(dyntracer_t* dyntracer, SEXP r_object);

void instrumentr_trace_variable_definition(dyntracer_t* dyntracer,
                                           const SEXP r_symbol,
                                           const SEXP r_value,
                                           const SEXP r_rho);

void instrumentr_trace_variable_assignment(dyntracer_t* dyntracer,
                                           const SEXP r_symbol,
                                           const SEXP r_value,
                                           const SEXP r_rho);

void instrumentr_trace_variable_removal(dyntracer_t* dyntracer,
                                        const SEXP r_symbol,
                                        const SEXP r_rho);

void instrumentr_trace_variable_lookup(dyntracer_t* dyntracer,
                                       const SEXP r_symbol,
                                       const SEXP r_value,
                                       const SEXP r_rho);

void instrumentr_trace_promise_force_entry(dyntracer_t* dyntracer,
                                           SEXP r_promise);

void instrumentr_trace_promise_force_exit(dyntracer_t* dyntracer,
                                          SEXP r_promise);

void instrumentr_trace_promise_value_lookup(dyntracer_t* dyntracer,
                                            SEXP r_promise);

void instrumentr_trace_promise_substitute(dyntracer_t* dyntracer,
                                          SEXP r_promise);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_TRACE_H */
