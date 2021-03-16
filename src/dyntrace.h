#ifndef INSTRUMENTR_DYNTRACE_H
#define INSTRUMENTR_DYNTRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include "tracer.h"

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer);

void instrumentr_dyntracer_destroy(dyntracer_t* dyntracer);

instrumentr_tracer_t instrumentr_dyntracer_get_tracer(dyntracer_t* dyntracer);

void dyntrace_basic_call_entry(dyntracer_t* dyntracer,
                               SEXP r_call,
                               SEXP r_op,
                               SEXP r_args,
                               SEXP r_rho,
                               dyntrace_dispatch_t dispatch);

void dyntrace_basic_call_exit(dyntracer_t* dyntracer,
                              SEXP r_call,
                              SEXP r_op,
                              SEXP r_args,
                              SEXP r_rho,
                              dyntrace_dispatch_t dispatch,
                              SEXP r_result);

void dyntrace_closure_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch);

void dyntrace_closure_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result);

void dyntrace_context_entry(dyntracer_t* dyntracer, void* pointer);

void dyntrace_context_exit(dyntracer_t* dyntracer, void* pointer);

void dyntrace_context_jump(dyntracer_t* dyntracer,
                           void* context,
                           const SEXP return_value,
                           int restart);

void dyntrace_eval_entry(dyntracer_t* dyntracer, SEXP r_expression, SEXP r_rho);

void dyntrace_eval_exit(dyntracer_t* dyntracer,
                        SEXP r_expression,
                        SEXP r_rho,
                        SEXP r_result);

void dyntrace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object);

void dyntrace_variable_definition(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho);

void dyntrace_variable_assignment(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho);

void dyntrace_variable_removal(dyntracer_t* dyntracer,
                               const SEXP r_symbol,
                               const SEXP r_rho);

void dyntrace_variable_lookup(dyntracer_t* dyntracer,
                              const SEXP r_symbol,
                              const SEXP r_value,
                              const SEXP r_rho);

void dyntrace_gc_unmark(dyntracer_t* dyntracer, SEXP r_object);

void dyntrace_promise_force_entry(dyntracer_t* dyntracer, SEXP r_promise);

void dyntrace_promise_force_exit(dyntracer_t* dyntracer, SEXP r_promise);

void dyntrace_promise_value_lookup(dyntracer_t* dyntracer, SEXP r_promise);

void dyntrace_promise_substitute(dyntracer_t* dyntracer, SEXP r_promise);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_DYNTRACE_H */
