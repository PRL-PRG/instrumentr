#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
/* lightr */
SEXP r_lightr_get_application();
SEXP r_lightr_get_context();
SEXP r_lightr_set_context(SEXP r_context);
SEXP r_lightr_interception_is_enabled();
SEXP r_lightr_disable_interception();
SEXP r_lightr_enable_interception();
SEXP r_lightr_initialize(SEXP r_application_name,
                         SEXP r_global_environment,
                         SEXP r_package_environment,
                         SEXP r_state_environment);
SEXP r_lightr_finalize();
SEXP r_lightr_intercept_package_entry(SEXP r_package);
SEXP r_lightr_intercept_package_exit(SEXP r_package);
SEXP r_lightr_intercept_call_entry(SEXP r_package_ptr,
                                   SEXP r_function_ptr,
                                   SEXP r_function_obj,
                                   SEXP r_call_env,
                                   SEXP r_caller_env);
SEXP r_lightr_intercept_call_exit(SEXP package_name,
                                  SEXP function_name,
                                  SEXP result,
                                  SEXP failed);

/* Object */
SEXP r_object_get_id(SEXP r_object);

/* Context */
SEXP r_context_create_context(SEXP r_initializer,
                              SEXP r_finalizer,
                              SEXP r_package_entry_callback,
                              SEXP r_package_exit_callback,
                              SEXP r_call_entry_callback,
                              SEXP r_call_exit_callback,
                              SEXP r_environment);
SEXP r_context_set_initializer(SEXP r_context, SEXP r_initializer);
SEXP r_context_get_initializer(SEXP r_context);
SEXP r_context_set_finalizer(SEXP r_context, SEXP r_finalizer);
SEXP r_context_get_finalizer(SEXP r_context);
SEXP r_context_set_package_entry_callback(SEXP r_context,
                                          SEXP r_package_entry_callback);
SEXP r_context_get_package_entry_callback(SEXP r_context);
SEXP r_context_set_package_exit_callback(SEXP r_context,
                                         SEXP r_package_exit_callback);
SEXP r_context_get_package_exit_callback(SEXP r_context);
SEXP r_context_set_call_entry_callback(SEXP r_context,
                                       SEXP r_call_entry_callback);
SEXP r_context_get_call_entry_callback(SEXP r_context);
SEXP r_context_set_call_exit_callback(SEXP r_context,
                                      SEXP r_call_exit_callback);
SEXP r_context_get_call_exit_callback(SEXP r_context);
SEXP r_context_set_environment(SEXP r_context, SEXP r_environment);
SEXP r_context_get_environment(SEXP r_context);

/* Application */
SEXP r_application_get_name(SEXP r_application);
SEXP r_application_get_global_environment(SEXP r_application);
SEXP r_application_get_packages(SEXP r_application);
SEXP r_application_add_package(SEXP r_application, SEXP r_package);

/* Package */
SEXP r_package_create(SEXP r_package_name,
                      SEXP r_package_directory,
                      SEXP r_package_environment);
SEXP r_package_get_name(SEXP r_package);
SEXP r_package_get_directory(SEXP r_package);
SEXP r_package_get_environment(SEXP r_package);
SEXP r_package_get_functions(SEXP r_package);
SEXP r_package_add_function(SEXP r_package, SEXP r_function);

/* Function */
SEXP r_function_create(SEXP r_function_name, SEXP r_parameter_count);
SEXP r_function_get_name(SEXP r_function);
SEXP r_function_get_parameter_count(SEXP r_function);

/* Call */
SEXP r_call_get_function(SEXP r_call);
SEXP r_call_get_state(SEXP r_call);
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

/* CallStack */
SEXP r_call_stack_get_size(SEXP r_stack);
SEXP r_call_stack_get_frame(SEXP r_stack, SEXP r_index);
SEXP r_call_stack_peek_frame(SEXP r_stack, SEXP r_index);
}
