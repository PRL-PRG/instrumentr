#ifndef INSTRUMENTR_DYNTRACE_H
#define INSTRUMENTR_DYNTRACE_H

#include <instrumentr/Rincludes.h>

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

/* TODO: implement and uncomment
void dyntrace_builtin_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch);

void dyntrace_builtin_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result);

void dyntrace_special_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch);

void dyntrace_special_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result);
*/

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

#endif /* INSTRUMENTR_DYNTRACE_H */
