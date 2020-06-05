#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
/* lightr */
SEXP r_lightr_is_tracing_enabled();
SEXP r_lightr_disable_tracing();
SEXP r_lightr_enable_tracing();
SEXP r_lightr_reinstate_tracing();
SEXP r_lightr_eval(SEXP r_code, SEXP r_environment);
SEXP r_lightr_initialize_lightr(SEXP r_package_environment,
                                SEXP r_state_environment,
                                SEXP r_invalid_value);
SEXP r_lightr_trace_application_entry(SEXP r_context, SEXP r_application);
SEXP r_lightr_trace_application_exit(SEXP r_context, SEXP r_application);
SEXP r_lightr_trace_package_entry(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package);
SEXP r_lightr_trace_package_exit(SEXP r_context,
                                 SEXP r_application,
                                 SEXP r_package);
SEXP r_lightr_trace_function_entry(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package,
                                   SEXP r_function);
SEXP r_lightr_trace_function_exit(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package,
                                  SEXP r_function);
SEXP r_lightr_trace_call_entry(SEXP r_context,
                               SEXP r_application,
                               SEXP r_package,
                               SEXP r_function,
                               SEXP r_call);
SEXP r_lightr_trace_call_exit(SEXP r_context,
                              SEXP r_application,
                              SEXP r_package,
                              SEXP r_function,
                              SEXP r_result);

/* Object */
SEXP r_object_get_id(SEXP r_object);
SEXP r_object_set_data(SEXP r_object, SEXP r_data);
SEXP r_object_get_data(SEXP r_object);
SEXP r_object_remove_data(SEXP r_object);
SEXP r_object_has_data(SEXP r_object);

/* Context */
SEXP r_context_create_context(SEXP r_application_entry_callback,
                              SEXP r_application_exit_callback,
                              SEXP r_package_entry_callback,
                              SEXP r_package_exit_callback,
                              SEXP r_function_entry_callback,
                              SEXP r_function_exit_callback,
                              SEXP r_call_entry_callback,
                              SEXP r_call_exit_callback,
                              SEXP r_environment);
SEXP r_context_set_application_entry_callback(
    SEXP r_context,
    SEXP r_application_entry_callback);
SEXP r_context_get_application_entry_callback(SEXP r_context);
SEXP r_context_has_application_entry_callback(SEXP r_context);
SEXP r_context_set_application_exit_callback(SEXP r_context,
                                             SEXP r_application_exit_callback);
SEXP r_context_get_application_exit_callback(SEXP r_context);
SEXP r_context_has_application_exit_callback(SEXP r_context);
SEXP r_context_set_package_entry_callback(SEXP r_context,
                                          SEXP r_package_entry_callback);
SEXP r_context_get_package_entry_callback(SEXP r_context);
SEXP r_context_has_package_entry_callback(SEXP r_context);
SEXP r_context_set_package_exit_callback(SEXP r_context,
                                         SEXP r_package_exit_callback);
SEXP r_context_get_package_exit_callback(SEXP r_context);
SEXP r_context_has_package_exit_callback(SEXP r_context);
SEXP r_context_set_function_entry_callback(SEXP r_context,
                                           SEXP r_function_entry_callback);
SEXP r_context_get_function_entry_callback(SEXP r_context);
SEXP r_context_has_function_entry_callback(SEXP r_context);
SEXP r_context_set_function_exit_callback(SEXP r_context,
                                          SEXP r_function_exit_callback);
SEXP r_context_get_function_exit_callback(SEXP r_context);
SEXP r_context_has_function_exit_callback(SEXP r_context);
SEXP r_context_set_call_entry_callback(SEXP r_context,
                                       SEXP r_call_entry_callback);
SEXP r_context_get_call_entry_callback(SEXP r_context);
SEXP r_context_has_call_entry_callback(SEXP r_context);
SEXP r_context_set_call_exit_callback(SEXP r_context,
                                      SEXP r_call_exit_callback);
SEXP r_context_get_call_exit_callback(SEXP r_context);
SEXP r_context_has_call_exit_callback(SEXP r_context);
SEXP r_context_set_environment(SEXP r_context, SEXP r_environment);
SEXP r_context_get_environment(SEXP r_context);
SEXP r_context_trace_package(SEXP r_context, SEXP r_package_name);
SEXP r_context_trace_function(SEXP r_context,
                              SEXP r_package_name,
                              SEXP r_function_name);
SEXP r_context_get_traced_packages(SEXP r_context);
SEXP r_context_get_traced_functions(SEXP r_context, SEXP r_package_name);
SEXP r_context_is_package_traced(SEXP r_context, SEXP r_package_name);
SEXP r_context_is_function_traced(SEXP r_context,
                                  SEXP r_package_name,
                                  SEXP r_function_name);

/* Application */
SEXP r_application_create_application(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment);
SEXP r_application_get_name(SEXP r_application);
SEXP r_application_get_directory(SEXP r_application);
SEXP r_application_get_code(SEXP r_application);
SEXP r_application_get_environment(SEXP r_application);
SEXP r_application_get_packages(SEXP r_application);
SEXP r_application_get_call_stack(SEXP r_application);
SEXP r_application_add_package(SEXP r_application, SEXP r_package);

/* Package */
SEXP r_package_create_package(SEXP r_package_name,
                              SEXP r_package_directory,
                              SEXP r_package_environment);
SEXP r_package_get_name(SEXP r_package);
SEXP r_package_get_directory(SEXP r_package);
SEXP r_package_get_environment(SEXP r_package);
SEXP r_package_get_functions(SEXP r_package);
SEXP r_package_add_function(SEXP r_package, SEXP r_function);

/* Function */
SEXP r_function_create_function(SEXP r_function_name,
                                SEXP r_parameter_count,
                                SEXP r_function_object);
SEXP r_function_get_name(SEXP r_function);
SEXP r_function_get_parameter_count(SEXP r_function);
SEXP r_function_get_definition(SEXP r_function);

/* Call */
SEXP r_call_create_call(SEXP r_function, SEXP r_call_obj, SEXP r_environment);
SEXP r_call_get_function(SEXP r_call);
SEXP r_call_get_expression(SEXP r_call);
SEXP r_call_get_environment(SEXP r_call);
SEXP r_call_is_active(SEXP r_call);
SEXP r_call_is_successful(SEXP r_call);
SEXP r_call_get_result(SEXP r_call);
SEXP r_call_get_parameters(SEXP r_call);

/* Parameter */
SEXP r_parameter_get_name(SEXP r_parameter);
SEXP r_parameter_get_position(SEXP r_parameter);
SEXP r_parameter_is_missing(SEXP r_parameter);
SEXP r_parameter_is_vararg(SEXP r_parameter);
SEXP r_parameter_get_arguments(SEXP r_parameter);

/* Argument */
SEXP r_argument_get_name(SEXP r_argument);
SEXP r_argument_is_evaluated(SEXP r_argument);
SEXP r_argument_get_expression(SEXP r_argument);
SEXP r_argument_get_result(SEXP r_argument);

/* CallStack */
SEXP r_call_stack_get_size(SEXP r_stack);
SEXP r_call_stack_peek_frame(SEXP r_stack, SEXP r_index);
}
