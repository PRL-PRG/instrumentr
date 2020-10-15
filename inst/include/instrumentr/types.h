#ifndef INSTRUMENTR_TYPES_H
#define INSTRUMENTR_TYPES_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/tracer.h>
#include <instrumentr/callback.h>
#include <instrumentr/application.h>
#include <instrumentr/package.h>
#include <instrumentr/function.h>
#include <instrumentr/call.h>
#include <instrumentr/parameter.h>
#include <instrumentr/argument.h>

typedef void (*application_load_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application);

typedef void (*application_unload_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application);

typedef void (*application_attach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application);

typedef void (*application_detach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application);

typedef void (*package_load_function_t)(instrumentr_tracer_t tracer,
                                        instrumentr_callback_t callback,
                                        instrumentr_application_t application,
                                        instrumentr_package_t package);

typedef void (*package_unload_function_t)(instrumentr_tracer_t tracer,
                                          instrumentr_callback_t callback,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package);

typedef void (*package_attach_function_t)(instrumentr_tracer_t tracer,
                                          instrumentr_callback_t callback,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package);

typedef void (*package_detach_function_t)(instrumentr_tracer_t tracer,
                                          instrumentr_callback_t callback,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package);

typedef void (*function_attach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    instrumentr_package_t package,
    instrumentr_function_t function);

typedef void (*function_detach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    instrumentr_package_t package,
    instrumentr_function_t function);

typedef void (*call_entry_function_t)(instrumentr_tracer_t tracer,
                                      instrumentr_callback_t callback,
                                      instrumentr_application_t application,
                                      instrumentr_package_t package,
                                      instrumentr_function_t function,
                                      instrumentr_call_t call);

typedef void (*call_exit_function_t)(instrumentr_tracer_t tracer,
                                     instrumentr_callback_t callback,
                                     instrumentr_application_t application,
                                     instrumentr_package_t package,
                                     instrumentr_function_t function,
                                     instrumentr_call_t call);

typedef void (*builtin_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho);

typedef void (*builtin_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho,
    SEXP r_result);

typedef void (*special_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho);

typedef void (*special_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho,
    SEXP r_result);

typedef void (*closure_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho);

typedef void (*closure_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_call,
    SEXP r_op,
    SEXP r_args,
    SEXP r_rho,
    SEXP r_result);

typedef void (*eval_entry_function_t)(instrumentr_tracer_t tracer,
                                      instrumentr_callback_t callback,
                                      instrumentr_application_t application,
                                      SEXP r_expression,
                                      SEXP r_rho);

typedef void (*eval_exit_function_t)(instrumentr_tracer_t tracer,
                                     instrumentr_callback_t callback,
                                     instrumentr_application_t application,
                                     SEXP r_expression,
                                     SEXP r_rho,
                                     SEXP r_result);

typedef void (*gc_allocation_function_t)(instrumentr_tracer_t tracer,
                                         instrumentr_callback_t callback,
                                         instrumentr_application_t application,
                                         SEXP r_object);

typedef void (*variable_definition_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

typedef void (*variable_assignment_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

typedef void (*variable_removal_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_rho);

typedef void (*variable_lookup_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

#endif /* INSTRUMENTR_TYPES_H */
