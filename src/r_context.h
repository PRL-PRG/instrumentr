#include "Rincludes.h"

extern "C" {
SEXP r_context_create_context(SEXP r_environment);
SEXP r_context_set_application_load_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_application_load_callback(SEXP r_context);
SEXP r_context_has_application_load_callback(SEXP r_context);
SEXP r_context_set_application_unload_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_application_unload_callback(SEXP r_context);
SEXP r_context_has_application_unload_callback(SEXP r_context);
SEXP r_context_set_application_attach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_application_attach_callback(SEXP r_context);
SEXP r_context_has_application_attach_callback(SEXP r_context);
SEXP r_context_set_application_detach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_application_detach_callback(SEXP r_context);
SEXP r_context_has_application_detach_callback(SEXP r_context);
SEXP r_context_set_package_load_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_package_load_callback(SEXP r_context);
SEXP r_context_has_package_load_callback(SEXP r_context);
SEXP r_context_set_package_unload_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_package_unload_callback(SEXP r_context);
SEXP r_context_has_package_unload_callback(SEXP r_context);
SEXP r_context_set_package_attach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_package_attach_callback(SEXP r_context);
SEXP r_context_has_package_attach_callback(SEXP r_context);
SEXP r_context_set_package_detach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_package_detach_callback(SEXP r_context);
SEXP r_context_has_package_detach_callback(SEXP r_context);
SEXP r_context_set_function_attach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_function_attach_callback(SEXP r_context);
SEXP r_context_has_function_attach_callback(SEXP r_context);
SEXP r_context_set_function_detach_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_function_detach_callback(SEXP r_context);
SEXP r_context_has_function_detach_callback(SEXP r_context);
SEXP r_context_set_call_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_call_entry_callback(SEXP r_context);
SEXP r_context_has_call_entry_callback(SEXP r_context);
SEXP r_context_set_call_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_call_exit_callback(SEXP r_context);
SEXP r_context_has_call_exit_callback(SEXP r_context);
SEXP r_context_set_object_coerce_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_object_coerce_callback(SEXP r_context);
SEXP r_context_has_object_coerce_callback(SEXP r_context);
SEXP r_context_set_object_duplicate_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_object_duplicate_callback(SEXP r_context);
SEXP r_context_has_object_duplicate_callback(SEXP r_context);
SEXP r_context_set_vector_copy_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_vector_copy_callback(SEXP r_context);
SEXP r_context_has_vector_copy_callback(SEXP r_context);
SEXP r_context_set_matrix_copy_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_matrix_copy_callback(SEXP r_context);
SEXP r_context_has_matrix_copy_callback(SEXP r_context);
SEXP r_context_set_builtin_call_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_builtin_call_entry_callback(SEXP r_context);
SEXP r_context_has_builtin_call_entry_callback(SEXP r_context);
SEXP r_context_set_builtin_call_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_builtin_call_exit_callback(SEXP r_context);
SEXP r_context_has_builtin_call_exit_callback(SEXP r_context);
SEXP r_context_set_special_call_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_special_call_entry_callback(SEXP r_context);
SEXP r_context_has_special_call_entry_callback(SEXP r_context);
SEXP r_context_set_special_call_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_special_call_exit_callback(SEXP r_context);
SEXP r_context_has_special_call_exit_callback(SEXP r_context);
SEXP r_context_set_closure_call_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_closure_call_entry_callback(SEXP r_context);
SEXP r_context_has_closure_call_entry_callback(SEXP r_context);
SEXP r_context_set_closure_call_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_closure_call_exit_callback(SEXP r_context);
SEXP r_context_has_closure_call_exit_callback(SEXP r_context);
SEXP r_context_set_eval_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_eval_entry_callback(SEXP r_context);
SEXP r_context_has_eval_entry_callback(SEXP r_context);
SEXP r_context_set_eval_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_eval_exit_callback(SEXP r_context);
SEXP r_context_has_eval_exit_callback(SEXP r_context);
SEXP r_context_set_gc_allocation_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_gc_allocation_callback(SEXP r_context);
SEXP r_context_has_gc_allocation_callback(SEXP r_context);
SEXP r_context_set_gc_unmark_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_gc_unmark_callback(SEXP r_context);
SEXP r_context_has_gc_unmark_callback(SEXP r_context);
SEXP r_context_set_variable_definition_callback(SEXP r_context,
                                                SEXP r_callback);
SEXP r_context_get_variable_definition_callback(SEXP r_context);
SEXP r_context_has_variable_definition_callback(SEXP r_context);
SEXP r_context_set_variable_assignment_callback(SEXP r_context,
                                                SEXP r_callback);
SEXP r_context_get_variable_assignment_callback(SEXP r_context);
SEXP r_context_has_variable_assignment_callback(SEXP r_context);
SEXP r_context_set_variable_removal_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_variable_removal_callback(SEXP r_context);
SEXP r_context_has_variable_removal_callback(SEXP r_context);
SEXP r_context_set_variable_lookup_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_variable_lookup_callback(SEXP r_context);
SEXP r_context_has_variable_lookup_callback(SEXP r_context);
SEXP r_context_set_context_entry_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_context_entry_callback(SEXP r_context);
SEXP r_context_has_context_entry_callback(SEXP r_context);
SEXP r_context_set_context_exit_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_context_exit_callback(SEXP r_context);
SEXP r_context_has_context_exit_callback(SEXP r_context);
SEXP r_context_set_context_jump_callback(SEXP r_context, SEXP r_callback);
SEXP r_context_get_context_jump_callback(SEXP r_context);
SEXP r_context_has_context_jump_callback(SEXP r_context);

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
SEXP r_context_is_tracing_enabled(SEXP r_context);
SEXP r_context_disable_tracing(SEXP r_context);
SEXP r_context_enable_tracing(SEXP r_context);
SEXP r_context_reinstate_tracing(SEXP r_context);
SEXP r_context_initialize_tracing(SEXP r_context);
SEXP r_context_finalize_tracing(SEXP r_context);
SEXP r_context_get_current_callback_type(SEXP r_context);

SEXP r_context_trace_code(SEXP r_context, SEXP r_code, SEXP r_environment);
SEXP r_context_trace_application_load(SEXP r_context, SEXP r_application);
SEXP r_context_trace_application_unload(SEXP r_context, SEXP r_application);
SEXP r_context_trace_application_attach(SEXP r_context, SEXP r_application);
SEXP r_context_trace_application_detach(SEXP r_context, SEXP r_application);
SEXP r_context_trace_package_load(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package);
SEXP r_context_trace_package_unload(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package);
SEXP r_context_trace_package_attach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package);
SEXP r_context_trace_package_detach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package);
SEXP r_context_trace_function_attach(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_package,
                                     SEXP r_function);
SEXP r_context_trace_function_detach(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_package,
                                     SEXP r_function);
SEXP r_context_trace_call_entry(SEXP r_context,
                                SEXP r_application,
                                SEXP r_package,
                                SEXP r_function,
                                SEXP r_call);
SEXP r_context_trace_call_exit(SEXP r_context,
                               SEXP r_application,
                               SEXP r_package,
                               SEXP r_function,
                               SEXP r_result);
}
