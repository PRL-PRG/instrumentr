#ifndef INSTRUMENTR_API_H
#define INSTRUMENTR_API_H

#include <instrumentr/types.h>

/********************************************************************************
 interop
 *******************************************************************************/

#define INSTRUMENTR_INTEROP_API_MAP(MACRO)                                    \
    MACRO(instrumentr_log_error, void, const char* fmt, ...)                  \
    MACRO(instrumentr_log_warning, void, const char* fmt, ...)                \
    MACRO(instrumentr_sexp_acquire, void, SEXP r_object)                      \
    MACRO(instrumentr_sexp_release, void, SEXP r_object)                      \
    MACRO(instrumentr_sexp_set_class, SEXP, SEXP r_object, SEXP r_class)      \
    MACRO(instrumentr_c_int_to_r_logical, SEXP, int value)                    \
    MACRO(instrumentr_r_logical_to_c_int, int, SEXP r_value)                  \
    MACRO(instrumentr_c_int_to_r_integer, SEXP, int value)                    \
    MACRO(instrumentr_r_integer_to_c_int, int, SEXP r_value)                  \
    MACRO(instrumentr_c_double_to_r_double, SEXP, double value)               \
    MACRO(instrumentr_r_double_to_c_double, double, SEXP r_value)             \
    MACRO(instrumentr_c_string_to_r_character, SEXP, const char* string)      \
    MACRO(instrumentr_r_character_to_c_string, const char*, SEXP r_character) \
    MACRO(instrumentr_c_pointer_to_r_externalptr,                             \
          SEXP,                                                               \
          void* pointer,                                                      \
          SEXP r_tag,                                                         \
          SEXP r_prot,                                                        \
          R_CFinalizer_t finalizer)                                           \
    MACRO(instrumentr_r_externalptr_to_c_pointer, void*, SEXP r_pointer)      \
    MACRO(instrumentr_r_externalptr_clear, void, SEXP r_externalptr)          \
    MACRO(instrumentr_create_list, SEXP, int column_count, ...)               \
    MACRO(instrumentr_create_data_frame, SEXP, int column_count, ...)

/********************************************************************************
 OBJECT API
 *******************************************************************************/

#define INSTRUMENTR_OBJECT_API_MAP(MACRO)                                  \
    MACRO(instrumentr_object_acquire, int, void* object)                   \
    MACRO(instrumentr_object_release, int, void* object)                   \
    MACRO(instrumentr_object_get_id, instrumentr_id_t, void* object)       \
    MACRO(r_instrumentr_object_get_id, SEXP, SEXP r_object)                \
    MACRO(instrumentr_object_has_data, int, void* object)                  \
    MACRO(r_instrumentr_object_has_data, SEXP, SEXP r_object)              \
    MACRO(instrumentr_object_set_data, void, void* object, SEXP r_data)    \
    MACRO(r_instrumentr_object_set_data, SEXP, SEXP r_object, SEXP r_data) \
    MACRO(instrumentr_object_get_data, SEXP, void* object)                 \
    MACRO(r_instrumentr_object_get_data, SEXP, SEXP r_object)              \
    MACRO(instrumentr_object_remove_data, void, void* object)              \
    MACRO(r_instrumentr_object_remove_data, SEXP, SEXP r_object)

/********************************************************************************
 APPLICATION API
 *******************************************************************************/

#define INSTRUMENTR_APPLICATION_API_MAP(MACRO)                                 \
    MACRO(instrumentr_application_wrap,                                        \
          SEXP,                                                                \
          instrumentr_application_t application)                               \
    MACRO(instrumentr_application_unwrap,                                      \
          instrumentr_application_t,                                           \
          SEXP r_application)                                                  \
    MACRO(instrumentr_application_get_name,                                    \
          const char*,                                                         \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_name, SEXP, SEXP r_application)        \
    MACRO(instrumentr_application_get_directory,                               \
          const char*,                                                         \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_directory, SEXP, SEXP r_application)   \
    MACRO(instrumentr_application_get_code,                                    \
          SEXP,                                                                \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_code, SEXP, SEXP r_application)        \
    MACRO(instrumentr_application_get_environment,                             \
          SEXP,                                                                \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_environment, SEXP, SEXP r_application) \
    MACRO(instrumentr_application_get_frame_position,                          \
          int,                                                                 \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_frame_position,                        \
          SEXP,                                                                \
          SEXP r_application)                                                  \
    MACRO(instrumentr_application_get_call_stack,                              \
          instrumentr_call_stack_t,                                            \
          instrumentr_application_t application)                               \
    MACRO(r_instrumentr_application_get_call_stack, SEXP, SEXP r_application)  \
    MACRO(instrumentr_application_get_package_count,                           \
          int,                                                                 \
          instrumentr_application_t application)                               \
    MACRO(                                                                     \
        r_instrumentr_application_get_package_count, SEXP, SEXP r_application) \
    MACRO(instrumentr_application_get_package_by_position,                     \
          instrumentr_package_t,                                               \
          instrumentr_application_t application,                               \
          int position)                                                        \
    MACRO(r_instrumentr_application_get_package_by_position,                   \
          SEXP,                                                                \
          SEXP r_application,                                                  \
          SEXP r_position)                                                     \
    MACRO(instrumentr_application_get_package_by_name,                         \
          instrumentr_package_t,                                               \
          instrumentr_application_t application,                               \
          const char* name)                                                    \
    MACRO(r_instrumentr_application_get_package_by_name,                       \
          SEXP,                                                                \
          SEXP r_application,                                                  \
          SEXP r_name)                                                         \
    MACRO(r_instrumentr_application_get_packages, SEXP, SEXP r_application)

/********************************************************************************
 PACKAGE API
 *******************************************************************************/

#define INSTRUMENTR_PACKAGE_API_MAP(MACRO)                                   \
    MACRO(instrumentr_package_wrap, SEXP, instrumentr_package_t package)     \
    MACRO(instrumentr_package_unwrap, instrumentr_package_t, SEXP r_package) \
    MACRO(instrumentr_package_get_name,                                      \
          const char*,                                                       \
          instrumentr_package_t package)                                     \
    MACRO(r_instrumentr_package_get_name, SEXP, SEXP r_package)              \
    MACRO(instrumentr_package_get_directory,                                 \
          const char*,                                                       \
          instrumentr_package_t package)                                     \
    MACRO(r_instrumentr_package_get_directory, SEXP, SEXP r_package)         \
    MACRO(instrumentr_package_get_namespace,                                 \
          SEXP,                                                              \
          instrumentr_package_t package)                                     \
    MACRO(r_instrumentr_package_get_namespace, SEXP, SEXP r_package)         \
    MACRO(instrumentr_package_get_function_count,                            \
          int,                                                               \
          instrumentr_package_t package)                                     \
    MACRO(r_instrumentr_package_get_function_count, SEXP, SEXP r_package)    \
    MACRO(instrumentr_package_get_function_by_position,                      \
          instrumentr_function_t,                                            \
          instrumentr_package_t package,                                     \
          int position)                                                      \
    MACRO(r_instrumentr_package_get_function_by_position,                    \
          SEXP,                                                              \
          SEXP r_package,                                                    \
          SEXP r_position)                                                   \
    MACRO(instrumentr_package_get_function_by_name,                          \
          instrumentr_function_t,                                            \
          instrumentr_package_t package,                                     \
          const char* name)                                                  \
    MACRO(r_instrumentr_package_get_function_by_name,                        \
          SEXP,                                                              \
          SEXP r_package,                                                    \
          SEXP r_name)                                                       \
    MACRO(r_instrumentr_package_get_functions, SEXP, SEXP r_package)

/********************************************************************************
 FUNCTION API
 *******************************************************************************/

#define INSTRUMENTR_FUNCTION_API_MAP(MACRO)                                   \
    MACRO(instrumentr_function_wrap, SEXP, instrumentr_function_t function)   \
    MACRO(                                                                    \
        instrumentr_function_unwrap, instrumentr_function_t, SEXP r_function) \
    MACRO(instrumentr_function_get_name,                                      \
          const char*,                                                        \
          instrumentr_function_t function)                                    \
    MACRO(r_instrumentr_function_get_name, SEXP, SEXP r_function)             \
    MACRO(instrumentr_function_get_definition,                                \
          SEXP,                                                               \
          instrumentr_function_t function)                                    \
    MACRO(r_instrumentr_function_get_definition, SEXP, SEXP r_function)       \
    MACRO(instrumentr_function_get_parameter_count,                           \
          int,                                                                \
          instrumentr_function_t function)                                    \
    MACRO(r_instrumentr_function_get_parameter_count, SEXP, SEXP r_function)  \
    MACRO(                                                                    \
        instrumentr_function_is_public, int, instrumentr_function_t function) \
    MACRO(r_instrumentr_function_is_public, SEXP, SEXP r_function)            \
    MACRO(instrumentr_function_is_s3_generic,                                 \
          int,                                                                \
          instrumentr_function_t function)                                    \
    MACRO(r_instrumentr_function_is_s3_generic, SEXP, SEXP r_function)        \
    MACRO(instrumentr_function_is_s3_method,                                  \
          int,                                                                \
          instrumentr_function_t function)                                    \
    MACRO(r_instrumentr_function_is_s3_method, SEXP, SEXP r_function)

/********************************************************************************
 CALL API
 *******************************************************************************/

#define INSTRUMENTR_CALL_API_MAP(MACRO)                                       \
    MACRO(instrumentr_call_wrap, SEXP, instrumentr_call_t call)               \
    MACRO(instrumentr_call_unwrap, instrumentr_call_t, SEXP r_call)           \
    MACRO(instrumentr_call_get_function,                                      \
          instrumentr_function_t,                                             \
          instrumentr_call_t call)                                            \
    MACRO(r_instrumentr_call_get_function, SEXP, SEXP r_call)                 \
    MACRO(instrumentr_call_get_expression, SEXP, instrumentr_call_t call)     \
    MACRO(r_instrumentr_call_get_expression, SEXP, SEXP r_call)               \
    MACRO(instrumentr_call_get_environment, SEXP, instrumentr_call_t call)    \
    MACRO(r_instrumentr_call_get_environment, SEXP, SEXP r_call)              \
    MACRO(instrumentr_call_get_frame_position, int, instrumentr_call_t call)  \
    MACRO(r_instrumentr_call_get_frame_position, SEXP, SEXP r_call)           \
    MACRO(instrumentr_call_is_active, int, instrumentr_call_t call)           \
    MACRO(r_instrumentr_call_is_active, SEXP, SEXP r_call)                    \
    MACRO(instrumentr_call_has_result, int, instrumentr_call_t call)          \
    MACRO(r_instrumentr_call_has_result, SEXP, SEXP r_call)                   \
    MACRO(instrumentr_call_get_result, SEXP, instrumentr_call_t call)         \
    MACRO(r_instrumentr_call_get_result, SEXP, SEXP r_call)                   \
    MACRO(instrumentr_call_get_parameter_count, int, instrumentr_call_t call) \
    MACRO(r_instrumentr_call_get_parameter_count, SEXP, SEXP r_call)          \
    MACRO(instrumentr_call_get_parameter_by_position,                         \
          instrumentr_parameter_t,                                            \
          instrumentr_call_t call,                                            \
          int position)                                                       \
    MACRO(r_instrumentr_call_get_parameter_by_position,                       \
          SEXP,                                                               \
          SEXP r_call,                                                        \
          SEXP r_position)                                                    \
    MACRO(instrumentr_call_get_parameter_by_name,                             \
          instrumentr_parameter_t,                                            \
          instrumentr_call_t call,                                            \
          const char* name)                                                   \
    MACRO(r_instrumentr_call_get_parameter_by_name,                           \
          SEXP,                                                               \
          SEXP r_call,                                                        \
          SEXP r_name)                                                        \
    MACRO(r_instrumentr_call_get_parameters, SEXP, SEXP r_call)

/********************************************************************************
 CALL STACK API
 *******************************************************************************/

#define INSTRUMENTR_CALL_STACK_API_MAP(MACRO)                                  \
    MACRO(instrumentr_call_stack_wrap,                                         \
          SEXP,                                                                \
          instrumentr_call_stack_t call_stack)                                 \
    MACRO(instrumentr_call_stack_unwrap,                                       \
          instrumentr_call_stack_t,                                            \
          SEXP r_call_stack)                                                   \
    MACRO(instrumentr_call_stack_get_size,                                     \
          int,                                                                 \
          instrumentr_call_stack_t call_stack)                                 \
    MACRO(r_instrumentr_call_stack_get_size, SEXP, SEXP r_call_stack)          \
    MACRO(instrumentr_call_stack_push,                                         \
          void,                                                                \
          instrumentr_call_stack_t call_stack,                                 \
          instrumentr_call_t call)                                             \
    MACRO(                                                                     \
        instrumentr_call_stack_pop, void, instrumentr_call_stack_t call_stack) \
    MACRO(instrumentr_call_stack_peek,                                         \
          instrumentr_call_t,                                                  \
          instrumentr_call_stack_t call_stack,                                 \
          int index)                                                           \
    MACRO(r_instrumentr_call_stack_peek, SEXP, SEXP r_call_stack, SEXP r_index)

/********************************************************************************
 PARAMETER API
 *******************************************************************************/

#define INSTRUMENTR_PARAMETER_API_MAP(MACRO)                                   \
    MACRO(instrumentr_parameter_wrap, SEXP, instrumentr_parameter_t parameter) \
    MACRO(instrumentr_parameter_unwrap,                                        \
          instrumentr_parameter_t,                                             \
          SEXP r_parameter)                                                    \
    MACRO(instrumentr_parameter_get_name,                                      \
          const char*,                                                         \
          instrumentr_parameter_t parameter)                                   \
    MACRO(r_instrumentr_parameter_get_name, SEXP, SEXP r_parameter)            \
    MACRO(instrumentr_parameter_get_position,                                  \
          int,                                                                 \
          instrumentr_parameter_t parameter)                                   \
    MACRO(r_instrumentr_parameter_get_position, SEXP, SEXP r_parameter)        \
    MACRO(instrumentr_parameter_has_default_argument,                          \
          int,                                                                 \
          instrumentr_parameter_t parameter)                                   \
    MACRO(                                                                     \
        r_instrumentr_parameter_has_default_argument, SEXP, SEXP r_parameter)  \
    MACRO(instrumentr_parameter_get_default_argument,                          \
          SEXP,                                                                \
          instrumentr_parameter_t parameter)                                   \
    MACRO(                                                                     \
        r_instrumentr_parameter_get_default_argument, SEXP, SEXP r_parameter)  \
    MACRO(instrumentr_parameter_get_argument_count,                            \
          int,                                                                 \
          instrumentr_parameter_t parameter)                                   \
    MACRO(r_instrumentr_parameter_get_argument_count, SEXP, SEXP r_parameter)  \
    MACRO(instrumentr_parameter_is_missing,                                    \
          int,                                                                 \
          instrumentr_parameter_t parameter)                                   \
    MACRO(r_instrumentr_parameter_is_missing, SEXP, SEXP r_parameter)          \
    MACRO(instrumentr_parameter_is_vararg,                                     \
          int,                                                                 \
          instrumentr_parameter_t parameter)                                   \
    MACRO(r_instrumentr_parameter_is_vararg, SEXP, SEXP r_parameter)           \
    MACRO(instrumentr_parameter_get_argument_by_position,                      \
          instrumentr_argument_t,                                              \
          instrumentr_parameter_t parameter,                                   \
          int position)                                                        \
    MACRO(r_instrumentr_parameter_get_argument_by_position,                    \
          SEXP,                                                                \
          SEXP r_parameter,                                                    \
          SEXP r_position)                                                     \
    MACRO(instrumentr_parameter_get_argument_by_name,                          \
          instrumentr_argument_t,                                              \
          instrumentr_parameter_t parameter,                                   \
          const char* name)                                                    \
    MACRO(r_instrumentr_parameter_get_argument_by_name,                        \
          SEXP,                                                                \
          SEXP r_parameter,                                                    \
          SEXP r_name)                                                         \
    MACRO(r_instrumentr_parameter_get_arguments, SEXP, SEXP r_parameter)

/********************************************************************************
 ARGUMENT API
 *******************************************************************************/

#define INSTRUMENTR_ARGUMENT_API_MAP(MACRO)                                    \
    MACRO(instrumentr_argument_wrap, SEXP, instrumentr_argument_t argument)    \
    MACRO(                                                                     \
        instrumentr_argument_unwrap, instrumentr_argument_t, SEXP r_argument)  \
    MACRO(instrumentr_argument_has_name, int, instrumentr_argument_t argument) \
    MACRO(r_instrumentr_argument_has_name, SEXP, SEXP r_argument)              \
    MACRO(instrumentr_argument_get_name,                                       \
          const char*,                                                         \
          instrumentr_argument_t argument)                                     \
    MACRO(r_instrumentr_argument_get_name, SEXP, SEXP r_argument)              \
    MACRO(instrumentr_argument_is_value, int, instrumentr_argument_t argument) \
    MACRO(r_instrumentr_argument_is_value, SEXP, SEXP r_argument);             \
    MACRO(instrumentr_argument_as_value,                                       \
          instrumentr_value_t,                                                 \
          instrumentr_argument_t argument)                                     \
    MACRO(r_instrumentr_argument_as_value, SEXP, SEXP r_argument)              \
    MACRO(                                                                     \
        instrumentr_argument_is_promise, int, instrumentr_argument_t argument) \
    MACRO(r_instrumentr_argument_is_promise, SEXP, SEXP r_argument)            \
    MACRO(instrumentr_argument_as_promise,                                     \
          instrumentr_promise_t,                                               \
          instrumentr_argument_t argument)                                     \
    MACRO(r_instrumentr_argument_as_promise, SEXP, SEXP r_argument)

/********************************************************************************
 PROMISE API
 *******************************************************************************/

#define INSTRUMENTR_PROMISE_API_MAP(MACRO)                                    \
    MACRO(instrumentr_promise_is_forced, int, instrumentr_promise_t promise)  \
    MACRO(r_instrumentr_promise_is_forced, SEXP, SEXP r_promise)              \
    MACRO(instrumentr_promise_get_expression,                                 \
          SEXP,                                                               \
          instrumentr_promise_t promise)                                      \
    MACRO(r_instrumentr_promise_get_expression, SEXP, SEXP r_promise)         \
    MACRO(instrumentr_promise_get_value, SEXP, instrumentr_promise_t promise) \
    MACRO(r_instrumentr_promise_get_value, SEXP, SEXP r_promise)

/********************************************************************************
 VALUE API
 *******************************************************************************/

#define INSTRUMENTR_VALUE_API_MAP(MACRO)                                      \
    MACRO(instrumentr_value_get_sexp, SEXP, instrumentr_value_t value)        \
    MACRO(r_instrumentr_value_get_sexp, SEXP, SEXP r_value)                   \
    MACRO(instrumentr_value_get_sexp_address,                                 \
          const char*,                                                        \
          instrumentr_value_t value)                                          \
    MACRO(r_instrumentr_value_get_sexp_address, SEXP, SEXP r_value)           \
    MACRO(                                                                    \
        instrumentr_value_get_sexp_type, SEXPTYPE, instrumentr_value_t value) \
    MACRO(r_instrumentr_value_get_sexp_type, SEXP, SEXP r_value)

/********************************************************************************
 CALLBACK API
 *******************************************************************************/

#define INSTRUMENTR_CALLBACK_API_MAP(MACRO)                                   \
    MACRO(instrumentr_callback_wrap, SEXP, instrumentr_callback_t callback)   \
    MACRO(                                                                    \
        instrumentr_callback_unwrap, instrumentr_callback_t, SEXP r_callback) \
    MACRO(instrumentr_callback_get_event,                                     \
          instrumentr_event_t,                                                \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_get_event, SEXP, SEXP r_callback)            \
    MACRO(instrumentr_callback_handles_event,                                 \
          instrumentr_event_t,                                                \
          instrumentr_callback_t callback,                                    \
          instrumentr_event_t event)                                          \
    MACRO(r_instrumentr_callback_handles_event,                               \
          SEXP,                                                               \
          SEXP r_callback,                                                    \
          SEXP r_event)                                                       \
    MACRO(instrumentr_callback_get_parameter_count,                           \
          int,                                                                \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_get_parameter_count, SEXP, SEXP r_callback)  \
    MACRO(instrumentr_callback_has_r_function,                                \
          int,                                                                \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_has_r_function, SEXP, SEXP r_callback)       \
    MACRO(instrumentr_callback_get_r_function,                                \
          SEXP,                                                               \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_get_r_function, SEXP, SEXP r_callback)       \
    MACRO(instrumentr_callback_has_c_function,                                \
          int,                                                                \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_has_c_function, SEXP, SEXP r_callback)       \
    MACRO(instrumentr_callback_get_c_function,                                \
          void*,                                                              \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_get_c_function, SEXP, SEXP r_callback)       \
    MACRO(                                                                    \
        instrumentr_callback_is_active, int, instrumentr_callback_t callback) \
    MACRO(r_instrumentr_callback_is_active, SEXP, SEXP r_callback)            \
    MACRO(                                                                    \
        instrumentr_callback_activate, void, instrumentr_callback_t callback) \
    MACRO(instrumentr_callback_deactivate,                                    \
          void,                                                               \
          instrumentr_callback_t callback)                                    \
    MACRO(instrumentr_callback_get_exec_stats,                                \
          instrumentr_exec_stats_t,                                           \
          instrumentr_callback_t callback)                                    \
    MACRO(r_instrumentr_callback_get_exec_stats, SEXP, SEXP r_callback)

/********************************************************************************
 TRACER API
 *******************************************************************************/

#define INSTRUMENTR_TRACER_API_MAP(MACRO)                                      \
    MACRO(instrumentr_tracer_create, instrumentr_tracer_t, )                   \
    MACRO(r_instrumentr_tracer_create, SEXP, )                                 \
    MACRO(instrumentr_tracer_wrap, SEXP, instrumentr_tracer_t tracer)          \
    MACRO(instrumentr_tracer_unwrap, instrumentr_tracer_t, SEXP r_tracer)      \
    MACRO(instrumentr_tracer_get_application,                                  \
          instrumentr_application_t,                                           \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_application, SEXP, SEXP r_tracer)           \
    MACRO(                                                                     \
        instrumentr_tracer_get_environment, SEXP, instrumentr_tracer_t tracer) \
    MACRO(r_instrumentr_tracer_get_environment, SEXP, SEXP r_tracer)           \
    MACRO(instrumentr_tracer_is_active, int, instrumentr_tracer_t tracer)      \
    MACRO(r_instrumentr_tracer_is_active, SEXP, SEXP r_tracer)                 \
    MACRO(instrumentr_tracer_get_active_callback,                              \
          instrumentr_callback_t,                                              \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_active_callback, SEXP, SEXP r_tracer)       \
    MACRO(instrumentr_tracer_set_active_callback,                              \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_callback_t callback)                                     \
    MACRO(instrumentr_tracer_unset_active_callback,                            \
          void,                                                                \
          instrumentr_tracer_t tracer)                                         \
    MACRO(instrumentr_tracer_is_enabled, int, instrumentr_tracer_t tracer)     \
    MACRO(r_instrumentr_tracer_is_enabled, SEXP, SEXP r_tracer)                \
    MACRO(instrumentr_tracer_enable, void, instrumentr_tracer_t tracer)        \
    MACRO(r_instrumentr_tracer_enable, SEXP, SEXP r_tracer)                    \
    MACRO(instrumentr_tracer_disable, void, instrumentr_tracer_t tracer)       \
    MACRO(r_instrumentr_tracer_disable, SEXP, SEXP r_tracer)                   \
    MACRO(instrumentr_tracer_reinstate, void, instrumentr_tracer_t tracer)     \
    MACRO(r_instrumentr_tracer_reinstate, SEXP, SEXP r_tracer)                 \
    MACRO(instrumentr_tracer_has_callback,                                     \
          int,                                                                 \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(                                                                     \
        r_instrumentr_tracer_has_callback, SEXP, SEXP r_tracer, SEXP r_event)  \
    MACRO(instrumentr_tracer_get_callback,                                     \
          instrumentr_callback_t,                                              \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(                                                                     \
        r_instrumentr_tracer_get_callback, SEXP, SEXP r_tracer, SEXP r_event)  \
    MACRO(instrumentr_tracer_set_callback,                                     \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_callback_t callback)                                     \
    MACRO(r_instrumentr_tracer_set_callback,                                   \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_callback)                                                     \
    MACRO(instrumentr_tracer_remove_callback,                                  \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(r_instrumentr_tracer_remove_callback,                                \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_event)                                                        \
    MACRO(instrumentr_tracer_get_tracing_exec_stats,                           \
          instrumentr_exec_stats_t,                                            \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_tracing_exec_stats, SEXP, SEXP r_tracer)    \
    MACRO(instrumentr_tracer_get_callback_exec_stats,                          \
          instrumentr_exec_stats_t,                                            \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(r_instrumentr_tracer_get_callback_exec_stats,                        \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_event)                                                        \
    MACRO(r_instrumentr_tracer_get_exec_stats, SEXP, SEXP r_tracer)

#define INSTRUMENTR_EXEC_STATS_API_MAP(MACRO)                                  \
    MACRO(instrumentr_exec_stats_wrap,                                         \
          SEXP,                                                                \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(instrumentr_exec_stats_unwrap,                                       \
          instrumentr_exec_stats_t,                                            \
          SEXP r_exec_stats)                                                   \
    MACRO(instrumentr_exec_stats_get_minimum_time,                             \
          double,                                                              \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(r_instrumentr_exec_stats_get_minimum_time, SEXP, SEXP r_exec_stats)  \
    MACRO(instrumentr_exec_stats_get_maximum_time,                             \
          double,                                                              \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(r_instrumentr_exec_stats_get_maximum_time, SEXP, SEXP r_exec_stats)  \
    MACRO(instrumentr_exec_stats_get_average_time,                             \
          double,                                                              \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(r_instrumentr_exec_stats_get_average_time, SEXP, SEXP r_exec_stats)  \
    MACRO(instrumentr_exec_stats_get_total_time,                               \
          double,                                                              \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(r_instrumentr_exec_stats_get_total_time, SEXP, SEXP r_exec_stats)    \
    MACRO(instrumentr_exec_stats_get_execution_count,                          \
          int,                                                                 \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(                                                                     \
        r_instrumentr_exec_stats_get_execution_count, SEXP, SEXP r_exec_stats) \
    MACRO(instrumentr_exec_stats_as_data_frame,                                \
          SEXP,                                                                \
          instrumentr_exec_stats_t exec_stats)                                 \
    MACRO(r_instrumentr_exec_stats_as_data_frame, SEXP, SEXP r_exec_stats)

#define INSTRUMENTR_API_MAP(MACRO)         \
    INSTRUMENTR_INTEROP_API_MAP(MACRO)     \
    INSTRUMENTR_OBJECT_API_MAP(MACRO)      \
    INSTRUMENTR_TRACER_API_MAP(MACRO)      \
    INSTRUMENTR_CALLBACK_API_MAP(MACRO)    \
    INSTRUMENTR_APPLICATION_API_MAP(MACRO) \
    INSTRUMENTR_CALL_STACK_API_MAP(MACRO)  \
    INSTRUMENTR_PACKAGE_API_MAP(MACRO)     \
    INSTRUMENTR_FUNCTION_API_MAP(MACRO)    \
    INSTRUMENTR_CALL_API_MAP(MACRO)        \
    INSTRUMENTR_PARAMETER_API_MAP(MACRO)   \
    INSTRUMENTR_ARGUMENT_API_MAP(MACRO)    \
    INSTRUMENTR_EXEC_STATS_API_MAP(MACRO)

#define INSTRUMENTR_API_DEFINER(FUNCTION, OUTPUT_TYPE, ...) \
    OUTPUT_TYPE (*FUNCTION)(__VA_ARGS__);

#define INSTRUMENTR_DEFINE_API() INSTRUMENTR_API_MAP(INSTRUMENTR_API_DEFINER)

#define INSTRUMENTR_API_DECLARATOR(FUNCTION, OUTPUT_TYPE, ...) \
    extern OUTPUT_TYPE (*FUNCTION)(__VA_ARGS__);

#define INSTRUMENTR_DECLARE_API() \
    INSTRUMENTR_API_MAP(INSTRUMENTR_API_DECLARATOR)

#define INSTRUMENTR_API_INITIALIZER(FUNCTION, OUTPUT_TYPE, ...) \
    FUNCTION = (OUTPUT_TYPE(*)(__VA_ARGS__))(                   \
        R_GetCCallable("instrumentr", #FUNCTION));

#define INSTRUMENTR_INITIALIZE_API() \
    INSTRUMENTR_API_MAP(INSTRUMENTR_API_INITIALIZER)

#define INSTRUMENTR_API_EXPORTER(FUNCTION, OUTPUT_TYPE, ...) \
    R_RegisterCCallable("instrumentr", #FUNCTION, (DL_FUNC)(FUNCTION));

#define INSTRUMENTR_EXPORT_API() INSTRUMENTR_API_MAP(INSTRUMENTR_API_EXPORTER)

#endif /* INSTRUMENTR_API_H */
