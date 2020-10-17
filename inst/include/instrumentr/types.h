#ifndef INSTRUMENTR_TYPES_H
#define INSTRUMENTR_TYPES_H

#include <instrumentr/Rincludes.h>

/*******************************************************************************
 * object
 *******************************************************************************/

typedef int instrumentr_id_t;

typedef struct instrumentr_object_impl_t* instrumentr_object_t;

/*******************************************************************************
 * application
 *******************************************************************************/

typedef struct instrumentr_application_impl_t* instrumentr_application_t;

/********************************************************************************
 * call stack
 *******************************************************************************/

typedef struct instrumentr_call_stack_impl_t* instrumentr_call_stack_t;

/********************************************************************************
 * package
 *******************************************************************************/

typedef struct instrumentr_package_impl_t* instrumentr_package_t;

/********************************************************************************
 * function
 *******************************************************************************/

typedef struct instrumentr_function_impl_t* instrumentr_function_t;

/********************************************************************************
 * call
 *******************************************************************************/

typedef struct instrumentr_call_impl_t* instrumentr_call_t;

/********************************************************************************
 * parameter
 *******************************************************************************/

typedef struct instrumentr_parameter_impl_t* instrumentr_parameter_t;

/********************************************************************************
 * argument
 *******************************************************************************/

typedef struct instrumentr_argument_impl_t* instrumentr_argument_t;

/********************************************************************************
 * callback
 *******************************************************************************/

typedef struct instrumentr_callback_impl_t* instrumentr_callback_t;

/********************************************************************************
 * tracer
 *******************************************************************************/

typedef struct instrumentr_tracer_impl_t* instrumentr_tracer_t;

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

#define INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(MACRO)                       \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_LOAD, application_load)       \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_UNLOAD, application_unload)   \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_ATTACH, application_attach)   \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_DETACH, application_detach)   \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_LOAD, package_load)               \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD, package_unload)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_ATTACH, package_attach)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_DETACH, package_detach)           \
    MACRO(INSTRUMENTR_CALLBACK_FUNCTION_ATTACH, function_attach)         \
    MACRO(INSTRUMENTR_CALLBACK_FUNCTION_DETACH, function_detach)         \
    MACRO(INSTRUMENTR_CALLBACK_CALL_ENTRY, call_entry)                   \
    MACRO(INSTRUMENTR_CALLBACK_CALL_EXIT, call_exit)                     \
    MACRO(INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY, builtin_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT, builtin_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY, special_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT, special_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY, closure_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT, closure_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_ENTRY, eval_entry)                   \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_EXIT, eval_exit)                     \
    MACRO(INSTRUMENTR_CALLBACK_GC_ALLOCATION, gc_allocation)             \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION, variable_definition) \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT, variable_assignment) \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL, variable_removal)       \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP, variable_lookup)

#endif /* INSTRUMENTR_TYPES_H */
