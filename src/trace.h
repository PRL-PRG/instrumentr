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

void instrumentr_trace_builtin_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch);

void instrumentr_trace_builtin_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result);

void instrumentr_trace_special_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch);

void instrumentr_trace_special_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result);

void instrumentr_trace_closure_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch);

void instrumentr_trace_closure_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result);

void instrumentr_trace_use_method_entry(dyntracer_t* dyntracer,
                                        const char* generic,
                                        SEXP r_klass,
                                        SEXP r_object,
                                        SEXP r_call,
                                        SEXP r_args,
                                        SEXP r_rho);

void instrumentr_trace_use_method_exit(dyntracer_t* dyntracer,
                                       const char* generic,
                                       SEXP r_klass,
                                       SEXP r_object,
                                       SEXP r_call,
                                       SEXP r_args,
                                       SEXP r_rho,
                                       SEXP r_ans);

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

void instrumentr_trace_eval_call_entry(dyntracer_t* dyntracer,
                                       SEXP r_expression,
                                       SEXP r_rho);

void instrumentr_trace_eval_call_exit(dyntracer_t* dyntracer,
                                      SEXP r_expression,
                                      SEXP r_rho,
                                      SEXP r_result);

void instrumentr_trace_function_context_lookup(dyntracer_t* dyntracer,
                                               const SEXP symbol,
                                               SEXP value,
                                               SEXP rho);

void instrumentr_trace_multivalue_finalize(SEXP r_tracer);

instrumentr_value_t
instrumentr_trace_value_finalize(instrumentr_tracer_t tracer,
                                 SEXP r_object,
                                 instrumentr_value_t value);

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

void instrumentr_trace_variable_exists(dyntracer_t* dyntracer,
                                       const SEXP r_symbol,
                                       const SEXP r_rho);

void instrumentr_trace_variable_lookup(dyntracer_t* dyntracer,
                                       const SEXP r_symbol,
                                       const SEXP r_value,
                                       const SEXP r_rho);

void instrumentr_trace_environment_ls(dyntracer_t* dyntracer,
                                      SEXP r_rho,
                                      Rboolean all,
                                      Rboolean sorted,
                                      SEXP r_result);

void instrumentr_trace_function_context_lookup(dyntracer_t* dyntracer,
                                               const SEXP r_symbol,
                                               const SEXP r_value,
                                               const SEXP r_rho);

void instrumentr_trace_promise_force_entry(dyntracer_t* dyntracer,
                                           SEXP r_promise);

void instrumentr_trace_promise_force_exit(dyntracer_t* dyntracer,
                                          SEXP r_promise);

void instrumentr_trace_promise_value_lookup(dyntracer_t* dyntracer,
                                            SEXP r_promise);

void instrumentr_trace_promise_expression_lookup(dyntracer_t* dyntracer,
                                                 SEXP r_promise);

void instrumentr_trace_promise_substitute(dyntracer_t* dyntracer,
                                          SEXP r_promise);

void instrumentr_trace_promise_delayed_assign(dyntracer_t* dyntracer,
                                              SEXP r_symbol,
                                              SEXP r_promise,
                                              SEXP r_rho);

void instrumentr_trace_promise_lazy_load(dyntracer_t* dyntracer,
                                         SEXP r_symbol,
                                         SEXP r_promise,
                                         SEXP r_rho);

void instrumentr_trace_error(dyntracer_t* dyntracer,
                             SEXP r_call,
                             const char* format,
                             va_list ap);

void instrumentr_trace_attribute_set(dyntracer_t* dyntracer,
                                     SEXP r_object,
                                     SEXP r_name,
                                     SEXP r_value);

void instrumentr_trace_subassign(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_x,
                                 SEXP r_index,
                                 SEXP r_y);

void instrumentr_trace_subset(dyntracer_t* dyntracer,
                              SEXP r_call,
                              SEXP r_x,
                              SEXP r_index,
                              SEXP r_result);
#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_TRACE_H */
