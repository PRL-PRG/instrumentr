#ifndef INSTRUMENTR_TYPES_H
#define INSTRUMENTR_TYPES_H

#include <instrumentr/Rincludes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int instrumentr_id_t;

/*******************************************************************************
 * object
 *******************************************************************************/

typedef struct instrumentr_object_impl_t* instrumentr_object_t;

/*******************************************************************************
 * model
 *******************************************************************************/

typedef struct instrumentr_model_impl_t* instrumentr_model_t;

/*******************************************************************************
 * state
 *******************************************************************************/

typedef struct instrumentr_state_impl_t* instrumentr_state_t;

/*******************************************************************************
 * application
 *******************************************************************************/

typedef struct instrumentr_application_impl_t* instrumentr_application_t;

/********************************************************************************
 * call stack
 *******************************************************************************/

typedef struct instrumentr_call_stack_impl_t* instrumentr_call_stack_t;

/********************************************************************************
 * environment
 *******************************************************************************/

typedef struct instrumentr_environment_impl_t* instrumentr_environment_t;

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
 * promise
 *******************************************************************************/

typedef struct instrumentr_promise_impl_t* instrumentr_promise_t;

/********************************************************************************
 * callback
 *******************************************************************************/

typedef struct instrumentr_callback_impl_t* instrumentr_callback_t;

/********************************************************************************
 * tracer
 *******************************************************************************/

typedef struct instrumentr_tracer_impl_t* instrumentr_tracer_t;

/********************************************************************************
 * frame
 *******************************************************************************/

typedef struct instrumentr_frame_impl_t* instrumentr_frame_t;

/********************************************************************************
 * context
 *******************************************************************************/

typedef struct instrumentr_context_impl_t* instrumentr_context_t;

/********************************************************************************
 * exec_stats
 *******************************************************************************/

typedef struct instrumentr_exec_stats_impl_t* instrumentr_exec_stats_t;

/********************************************************************************
 * alloc_stats
 *******************************************************************************/

typedef struct instrumentr_alloc_stats_impl_t* instrumentr_alloc_stats_t;

/********************************************************************************
 * miscellaneous
 *******************************************************************************/

typedef struct instrumentr_miscellaneous_impl_t* instrumentr_miscellaneous_t;

/********************************************************************************
 * unbound
 *******************************************************************************/

typedef struct instrumentr_unbound_impl_t* instrumentr_unbound_t;

/********************************************************************************
 * missing
 *******************************************************************************/

typedef struct instrumentr_missing_impl_t* instrumentr_missing_t;

/********************************************************************************
 * null
 *******************************************************************************/

typedef struct instrumentr_null_impl_t* instrumentr_null_t;

/********************************************************************************
 * externalptr
 *******************************************************************************/

typedef struct instrumentr_externalptr_impl_t* instrumentr_externalptr_t;

/********************************************************************************
 * weakref
 *******************************************************************************/

typedef struct instrumentr_weakref_impl_t* instrumentr_weakref_t;

/********************************************************************************
 * bytecode
 *******************************************************************************/

typedef struct instrumentr_bytecode_impl_t* instrumentr_bytecode_t;

/********************************************************************************
 * s4
 *******************************************************************************/

typedef struct instrumentr_s4_impl_t* instrumentr_s4_t;

/********************************************************************************
 * char
 *******************************************************************************/

typedef struct instrumentr_char_impl_t* instrumentr_char_t;

/********************************************************************************
 * symbol
 *******************************************************************************/

typedef struct instrumentr_symbol_impl_t* instrumentr_symbol_t;

/********************************************************************************
 * character
 *******************************************************************************/

typedef struct instrumentr_character_impl_t* instrumentr_character_t;

/********************************************************************************
 * integer
 *******************************************************************************/

typedef struct instrumentr_integer_impl_t* instrumentr_integer_t;

/********************************************************************************
 * real
 *******************************************************************************/

typedef struct instrumentr_real_impl_t* instrumentr_real_t;

/********************************************************************************
 * logical
 *******************************************************************************/

typedef struct instrumentr_logical_impl_t* instrumentr_logical_t;

/********************************************************************************
 * complex
 *******************************************************************************/

typedef struct instrumentr_complex_impl_t* instrumentr_complex_t;

/********************************************************************************
 * raw
 *******************************************************************************/

typedef struct instrumentr_raw_impl_t* instrumentr_raw_t;

/********************************************************************************
 * value
 *******************************************************************************/

typedef struct {
    instrumentr_model_t model;
} instrumentr_value_t;

/* The type of the do_xxxx functions. */
/* These are the built-in R functions. */
typedef SEXP (*CCODE)(SEXP, SEXP, SEXP, SEXP);

typedef enum {
    INSTRUMENTR_OBJECT_TYPE_TRACER = 0,
    INSTRUMENTR_OBJECT_TYPE_CALLBACK,
    INSTRUMENTR_OBJECT_TYPE_STATE,
    INSTRUMENTR_OBJECT_TYPE_EXEC_STATS,
    INSTRUMENTR_OBJECT_TYPE_ALLOC_STATS,
    /* NOTE: this has to be the last one always */
    INSTRUMENTR_OBJECT_TYPE_COUNT
} instrumentr_object_type_t;

typedef enum {
    INSTRUMENTR_MODEL_TYPE_APPLICATION = 0,
    INSTRUMENTR_MODEL_TYPE_UNBOUND,
    INSTRUMENTR_MODEL_TYPE_MISSING,
    INSTRUMENTR_MODEL_TYPE_NULL,
    INSTRUMENTR_MODEL_TYPE_EXTERNALPTR,
    INSTRUMENTR_MODEL_TYPE_WEAKREF,
    INSTRUMENTR_MODEL_TYPE_BYTECODE,
    INSTRUMENTR_MODEL_TYPE_S4,
    INSTRUMENTR_MODEL_TYPE_CHAR,
    INSTRUMENTR_MODEL_TYPE_SYMBOL,
    INSTRUMENTR_MODEL_TYPE_CHARACTER,
    INSTRUMENTR_MODEL_TYPE_INTEGER,
    INSTRUMENTR_MODEL_TYPE_REAL,
    INSTRUMENTR_MODEL_TYPE_LOGICAL,
    INSTRUMENTR_MODEL_TYPE_COMPLEX,
    INSTRUMENTR_MODEL_TYPE_RAW,
    INSTRUMENTR_MODEL_TYPE_ENVIRONMENT,
    INSTRUMENTR_MODEL_TYPE_FUNCTION,
    INSTRUMENTR_MODEL_TYPE_CALL,
    INSTRUMENTR_MODEL_TYPE_CALL_STACK,
    INSTRUMENTR_MODEL_TYPE_PARAMETER,
    INSTRUMENTR_MODEL_TYPE_ARGUMENT,
    INSTRUMENTR_MODEL_TYPE_PROMISE,
    INSTRUMENTR_MODEL_TYPE_FRAME,
    INSTRUMENTR_MODEL_TYPE_CONTEXT,
    INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS,
    /* NOTE: this has to be the last one always */
    INSTRUMENTR_MODEL_TYPE_COUNT
} instrumentr_model_type_t;

typedef enum {
    INSTRUMENTR_ORIGIN_LOCAL,
    INSTRUMENTR_ORIGIN_FOREIGN
} instrumentr_origin_t;

typedef enum {
    INSTRUMENTR_FUNCTION_BUILTIN = 0,
    INSTRUMENTR_FUNCTION_CLOSURE,
    INSTRUMENTR_FUNCTION_SPECIAL
} instrumentr_function_type_t;

typedef enum {
    INSTRUMENTR_PROMISE_TYPE_ARGUMENT,
    INSTRUMENTR_PROMISE_TYPE_DELAYED_ASSIGN,
    INSTRUMENTR_PROMISE_TYPE_LAZY_LOAD,
    INSTRUMENTR_PROMISE_TYPE_UNKNOWN
} instrumentr_promise_type_t;

typedef union {
    SEXP sexp;
    CCODE ccode;
} instrumentr_function_definition_t;

typedef enum {
    INSTRUMENTR_EVENT_TRACING_ENTRY = 0,
    INSTRUMENTR_EVENT_TRACING_EXIT,
    INSTRUMENTR_EVENT_PACKAGE_LOAD,
    INSTRUMENTR_EVENT_PACKAGE_UNLOAD,
    INSTRUMENTR_EVENT_PACKAGE_ATTACH,
    INSTRUMENTR_EVENT_PACKAGE_DETACH,
    INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY,
    INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
    INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY,
    INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
    INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY,
    INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
    INSTRUMENTR_EVENT_EVAL_ENTRY,
    INSTRUMENTR_EVENT_EVAL_EXIT,
    INSTRUMENTR_EVENT_GC_ALLOCATION,
    INSTRUMENTR_EVENT_GC_DEALLOCATION,
    INSTRUMENTR_EVENT_VARIABLE_DEFINITION,
    INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT,
    INSTRUMENTR_EVENT_VARIABLE_REMOVAL,
    INSTRUMENTR_EVENT_VARIABLE_LOOKUP,
    INSTRUMENTR_EVENT_CONTEXT_ENTRY,
    INSTRUMENTR_EVENT_CONTEXT_EXIT,
    INSTRUMENTR_EVENT_PROMISE_FORCE_ENTRY,
    INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT,
    INSTRUMENTR_EVENT_PROMISE_VALUE_LOOKUP,
    INSTRUMENTR_EVENT_PROMISE_SUBSTITUTE,
    /* NOTE: this has to be the last event */
    INSTRUMENTR_EVENT_COUNT
} instrumentr_event_t;

typedef enum {
    INSTRUMENTR_ENVIRONMENT_TYPE_UNKNOWN = 0,
    INSTRUMENTR_ENVIRONMENT_TYPE_NAMESPACE,
    INSTRUMENTR_ENVIRONMENT_TYPE_PACKAGE,
    INSTRUMENTR_ENVIRONMENT_TYPE_COUNT
} instrumentr_environment_type_t;

typedef void (*tracing_entry_function_t)(instrumentr_tracer_t tracer,
                                         instrumentr_callback_t callback,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application);

typedef void (*tracing_exit_function_t)(instrumentr_tracer_t tracer,
                                        instrumentr_callback_t callback,
                                        instrumentr_state_t state,
                                        instrumentr_application_t application);

typedef void (*package_load_function_t)(instrumentr_tracer_t tracer,
                                        instrumentr_callback_t callback,
                                        instrumentr_state_t state,
                                        instrumentr_application_t application,
                                        instrumentr_environment_t environment);

typedef void (*package_unload_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_environment_t environment);

typedef void (*package_attach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_environment_t environment);

typedef void (*package_detach_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_environment_t environment);

typedef void (*builtin_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*builtin_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*special_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*special_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*closure_call_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*closure_call_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_function_t function,
    instrumentr_call_t call);

typedef void (*eval_entry_function_t)(instrumentr_tracer_t tracer,
                                      instrumentr_callback_t callback,
                                      instrumentr_state_t state,
                                      instrumentr_application_t application,
                                      SEXP r_expression,
                                      SEXP r_rho);

typedef void (*eval_exit_function_t)(instrumentr_tracer_t tracer,
                                     instrumentr_callback_t callback,
                                     instrumentr_state_t state,
                                     instrumentr_application_t application,
                                     SEXP r_expression,
                                     SEXP r_rho,
                                     SEXP r_result);

typedef void (*gc_allocation_function_t)(instrumentr_tracer_t tracer,
                                         instrumentr_callback_t callback,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application,
                                         SEXP r_object);

typedef void (*variable_definition_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

typedef void (*variable_assignment_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

typedef void (*variable_removal_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_rho);

typedef void (*variable_lookup_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    SEXP r_variable,
    SEXP r_value,
    SEXP r_rho);

typedef void (*context_entry_function_t)(instrumentr_tracer_t tracer,
                                         instrumentr_callback_t callback,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application,
                                         instrumentr_context_t context);

typedef void (*context_exit_function_t)(instrumentr_tracer_t tracer,
                                        instrumentr_callback_t callback,
                                        instrumentr_state_t state,
                                        instrumentr_application_t application,
                                        instrumentr_context_t context);

typedef void (*promise_force_entry_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_promise_t promise);

typedef void (*promise_force_exit_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_promise_t promise);

typedef void (*promise_value_lookup_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_promise_t promise);

typedef void (*promise_substitute_function_t)(
    instrumentr_tracer_t tracer,
    instrumentr_callback_t callback,
    instrumentr_state_t state,
    instrumentr_application_t application,
    instrumentr_promise_t promise);

#define INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO_1(MACRO, ARGUMENT)                 \
    MACRO(INSTRUMENTR_CALLBACK_TRACING_ENTRY, tracing_entry, ARGUMENT)         \
    MACRO(INSTRUMENTR_CALLBACK_TRACING_EXIT, tracing_exit, ARGUMENT)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_LOAD, package_load, ARGUMENT)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD, package_unload, ARGUMENT)       \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_ATTACH, package_attach, ARGUMENT)       \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_DETACH, package_detach, ARGUMENT)       \
    MACRO(                                                                     \
        INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY, builtin_call_entry, ARGUMENT) \
    MACRO(INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT, builtin_call_exit, ARGUMENT) \
    MACRO(                                                                     \
        INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY, special_call_entry, ARGUMENT) \
    MACRO(INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT, special_call_exit, ARGUMENT) \
    MACRO(                                                                     \
        INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY, closure_call_entry, ARGUMENT) \
    MACRO(INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT, closure_call_exit, ARGUMENT) \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_ENTRY, eval_entry, ARGUMENT)               \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_EXIT, eval_exit, ARGUMENT)                 \
    MACRO(INSTRUMENTR_CALLBACK_GC_ALLOCATION, gc_allocation, ARGUMENT)         \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION,                            \
          variable_definition,                                                 \
          ARGUMENT)                                                            \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT,                            \
          variable_assignment,                                                 \
          ARGUMENT)                                                            \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL, variable_removal, ARGUMENT)   \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP, variable_lookup, ARGUMENT)

#define INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(MACRO) \
    INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO_1(MACRO, NULL)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_TYPES_H */
