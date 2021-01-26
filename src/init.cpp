#include "r_instrumentr.h"
#include "r_object.h"
#include "r_context.h"
#include "r_application.h"
#include "r_package.h"
#include "r_function.h"
#include "r_call.h"
#include "r_parameter.h"
#include "r_argument.h"
#include "r_call_stack.h"
#include "r_callback.h"
#include "r_application_attach_callback.h"
#include "r_application_detach_callback.h"
#include "r_application_load_callback.h"
#include "r_application_unload_callback.h"
#include "r_package_attach_callback.h"
#include "r_package_detach_callback.h"
#include "r_package_load_callback.h"
#include "r_package_unload_callback.h"
#include "r_function_attach_callback.h"
#include "r_function_detach_callback.h"
#include "r_call_entry_callback.h"
#include "r_call_exit_callback.h"
#include "r_object_coerce_callback.h"
#include "r_object_duplicate_callback.h"
#include "r_vector_copy_callback.h"
#include "r_matrix_copy_callback.h"
#include "r_builtin_call_entry_callback.h"
#include "r_builtin_call_exit_callback.h"
#include "r_special_call_entry_callback.h"
#include "r_special_call_exit_callback.h"
#include "r_closure_call_entry_callback.h"
#include "r_closure_call_exit_callback.h"
#include "r_eval_entry_callback.h"
#include "r_eval_exit_callback.h"
#include "r_gc_allocation_callback.h"
#include "r_gc_unmark_callback.h"
#include "r_variable_definition_callback.h"
#include "r_variable_assignment_callback.h"
#include "r_variable_removal_callback.h"
#include "r_variable_lookup_callback.h"

#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* instrumentr */
    {"instrumentr_get_commit_hash", (DL_FUNC) &r_instrumentr_get_commit_hash, 0},
    {"instrumentr_is_tracing_enabled", (DL_FUNC) &r_instrumentr_is_tracing_enabled, 0},
    {"instrumentr_disable_tracing", (DL_FUNC) &r_instrumentr_disable_tracing, 0},
    {"instrumentr_enable_tracing", (DL_FUNC) &r_instrumentr_enable_tracing, 0},
    {"instrumentr_reinstate_tracing", (DL_FUNC) &r_instrumentr_reinstate_tracing, 0},
    {"instrumentr_initialize_instrumentr", (DL_FUNC) &r_instrumentr_initialize_instrumentr, 3},
    {"instrumentr_finalize_instrumentr", (DL_FUNC) &r_instrumentr_finalize_instrumentr, 0},
    {"instrumentr_is_undefined_object", (DL_FUNC)&r_instrumentr_is_undefined_object, 1},
    {"instrumentr_is_defined_object", (DL_FUNC)&r_instrumentr_is_defined_object, 1},
    {"instrumentr_get_object_details", (DL_FUNC)&r_instrumentr_get_object_details, 4},

    /* Object */
    {"object_get_id", (DL_FUNC) &r_object_get_id, 1},
    {"object_set_data", (DL_FUNC) &r_object_set_data, 2},
    {"object_get_data", (DL_FUNC) &r_object_get_data, 1},
    {"object_remove_data", (DL_FUNC) &r_object_remove_data, 1},
    {"object_has_data", (DL_FUNC) &r_object_has_data, 1},

    /* Context */
    {"context_create_context", (DL_FUNC) &r_context_create_context, 1},
    {"context_set_application_load_callback", (DL_FUNC) &r_context_set_application_load_callback, 2},
    {"context_get_application_load_callback", (DL_FUNC) &r_context_get_application_load_callback, 1},
    {"context_has_application_load_callback", (DL_FUNC) &r_context_has_application_load_callback, 1},
    {"context_set_application_unload_callback", (DL_FUNC) &r_context_set_application_unload_callback, 2},
    {"context_get_application_unload_callback", (DL_FUNC) &r_context_get_application_unload_callback, 1},
    {"context_has_application_unload_callback", (DL_FUNC) &r_context_has_application_unload_callback, 1},
    {"context_set_application_attach_callback", (DL_FUNC) &r_context_set_application_attach_callback, 2},
    {"context_get_application_attach_callback", (DL_FUNC) &r_context_get_application_attach_callback, 1},
    {"context_has_application_attach_callback", (DL_FUNC) &r_context_has_application_attach_callback, 1},
    {"context_set_application_detach_callback", (DL_FUNC) &r_context_set_application_detach_callback, 2},
    {"context_get_application_detach_callback", (DL_FUNC) &r_context_get_application_detach_callback, 1},
    {"context_has_application_detach_callback", (DL_FUNC) &r_context_has_application_detach_callback, 1},
    {"context_set_package_load_callback", (DL_FUNC) &r_context_set_package_load_callback, 2},
    {"context_get_package_load_callback", (DL_FUNC) &r_context_get_package_load_callback, 1},
    {"context_has_package_load_callback", (DL_FUNC) &r_context_has_package_load_callback, 1},
    {"context_set_package_unload_callback", (DL_FUNC) &r_context_set_package_unload_callback, 2},
    {"context_get_package_unload_callback", (DL_FUNC) &r_context_get_package_unload_callback, 1},
    {"context_has_package_unload_callback", (DL_FUNC) &r_context_has_package_unload_callback, 1},
    {"context_set_package_attach_callback", (DL_FUNC) &r_context_set_package_attach_callback, 2},
    {"context_get_package_attach_callback", (DL_FUNC) &r_context_get_package_attach_callback, 1},
    {"context_has_package_attach_callback", (DL_FUNC) &r_context_has_package_attach_callback, 1},
    {"context_set_package_detach_callback", (DL_FUNC) &r_context_set_package_detach_callback, 2},
    {"context_get_package_detach_callback", (DL_FUNC) &r_context_get_package_detach_callback, 1},
    {"context_has_package_detach_callback", (DL_FUNC) &r_context_has_package_detach_callback, 1},
    {"context_set_function_attach_callback", (DL_FUNC) &r_context_set_function_attach_callback, 2},
    {"context_get_function_attach_callback", (DL_FUNC) &r_context_get_function_attach_callback, 1},
    {"context_has_function_attach_callback", (DL_FUNC) &r_context_has_function_attach_callback, 1},
    {"context_set_function_detach_callback", (DL_FUNC) &r_context_set_function_detach_callback, 2},
    {"context_get_function_detach_callback", (DL_FUNC) &r_context_get_function_detach_callback, 1},
    {"context_has_function_detach_callback", (DL_FUNC) &r_context_has_function_detach_callback, 1},
    {"context_set_call_entry_callback", (DL_FUNC) &r_context_set_call_entry_callback, 2},
    {"context_get_call_entry_callback", (DL_FUNC) &r_context_get_call_entry_callback, 1},
    {"context_has_call_entry_callback", (DL_FUNC) &r_context_has_call_entry_callback, 1},
    {"context_set_call_exit_callback", (DL_FUNC) &r_context_set_call_exit_callback, 2},
    {"context_get_call_exit_callback", (DL_FUNC) &r_context_get_call_exit_callback, 1},
    {"context_has_call_exit_callback", (DL_FUNC) &r_context_has_call_exit_callback, 1},
    {"context_set_object_coerce_callback", (DL_FUNC) &r_context_set_object_coerce_callback, 2},
    {"context_get_object_coerce_callback", (DL_FUNC) &r_context_get_object_coerce_callback, 1},
    {"context_has_object_coerce_callback", (DL_FUNC) &r_context_has_object_coerce_callback, 1},
    {"context_set_object_duplicate_callback", (DL_FUNC) &r_context_set_object_duplicate_callback, 2},
    {"context_get_object_duplicate_callback", (DL_FUNC) &r_context_get_object_duplicate_callback, 1},
    {"context_has_object_duplicate_callback", (DL_FUNC) &r_context_has_object_duplicate_callback, 1},
    {"context_set_vector_copy_callback", (DL_FUNC) &r_context_set_vector_copy_callback, 2},
    {"context_get_vector_copy_callback", (DL_FUNC) &r_context_get_vector_copy_callback, 1},
    {"context_has_vector_copy_callback", (DL_FUNC) &r_context_has_vector_copy_callback, 1},
    {"context_set_matrix_copy_callback", (DL_FUNC) &r_context_set_matrix_copy_callback, 2},
    {"context_get_matrix_copy_callback", (DL_FUNC) &r_context_get_matrix_copy_callback, 1},
    {"context_has_matrix_copy_callback", (DL_FUNC) &r_context_has_matrix_copy_callback, 1},
    {"context_set_builtin_call_entry_callback", (DL_FUNC) &r_context_set_builtin_call_entry_callback, 2},
    {"context_get_builtin_call_entry_callback", (DL_FUNC) &r_context_get_builtin_call_entry_callback, 1},
    {"context_has_builtin_call_entry_callback", (DL_FUNC) &r_context_has_builtin_call_entry_callback, 1},
    {"context_set_builtin_call_exit_callback", (DL_FUNC) &r_context_set_builtin_call_exit_callback, 2},
    {"context_get_builtin_call_exit_callback", (DL_FUNC) &r_context_get_builtin_call_exit_callback, 1},
    {"context_has_builtin_call_exit_callback", (DL_FUNC) &r_context_has_builtin_call_exit_callback, 1},
    {"context_set_special_call_entry_callback", (DL_FUNC) &r_context_set_special_call_entry_callback, 2},
    {"context_get_special_call_entry_callback", (DL_FUNC) &r_context_get_special_call_entry_callback, 1},
    {"context_has_special_call_entry_callback", (DL_FUNC) &r_context_has_special_call_entry_callback, 1},
    {"context_set_special_call_exit_callback", (DL_FUNC) &r_context_set_special_call_exit_callback, 2},
    {"context_get_special_call_exit_callback", (DL_FUNC) &r_context_get_special_call_exit_callback, 1},
    {"context_has_special_call_exit_callback", (DL_FUNC) &r_context_has_special_call_exit_callback, 1},
    {"context_set_closure_call_entry_callback", (DL_FUNC) &r_context_set_closure_call_entry_callback, 2},
    {"context_get_closure_call_entry_callback", (DL_FUNC) &r_context_get_closure_call_entry_callback, 1},
    {"context_has_closure_call_entry_callback", (DL_FUNC) &r_context_has_closure_call_entry_callback, 1},
    {"context_set_closure_call_exit_callback", (DL_FUNC) &r_context_set_closure_call_exit_callback, 2},
    {"context_get_closure_call_exit_callback", (DL_FUNC) &r_context_get_closure_call_exit_callback, 1},
    {"context_has_closure_call_exit_callback", (DL_FUNC) &r_context_has_closure_call_exit_callback, 1},
    {"context_set_eval_entry_callback", (DL_FUNC) &r_context_set_eval_entry_callback, 2},
    {"context_get_eval_entry_callback", (DL_FUNC) &r_context_get_eval_entry_callback, 1},
    {"context_has_eval_entry_callback", (DL_FUNC) &r_context_has_eval_entry_callback, 1},
    {"context_set_eval_exit_callback", (DL_FUNC) &r_context_set_eval_exit_callback, 2},
    {"context_get_eval_exit_callback", (DL_FUNC) &r_context_get_eval_exit_callback, 1},
    {"context_has_eval_exit_callback", (DL_FUNC) &r_context_has_eval_exit_callback, 1},
    {"context_set_gc_allocation_callback", (DL_FUNC) &r_context_set_gc_allocation_callback, 2},
    {"context_get_gc_allocation_callback", (DL_FUNC) &r_context_get_gc_allocation_callback, 1},
    {"context_has_gc_allocation_callback", (DL_FUNC) &r_context_has_gc_allocation_callback, 1},
    {"context_set_gc_unmark_callback", (DL_FUNC) &r_context_set_gc_unmark_callback, 2},
    {"context_get_gc_unmark_callback", (DL_FUNC) &r_context_get_gc_unmark_callback, 1},
    {"context_has_gc_unmark_callback", (DL_FUNC) &r_context_has_gc_unmark_callback, 1},
    {"context_set_variable_definition_callback", (DL_FUNC) &r_context_set_variable_definition_callback, 2},
    {"context_get_variable_definition_callback", (DL_FUNC) &r_context_get_variable_definition_callback, 1},
    {"context_has_variable_definition_callback", (DL_FUNC) &r_context_has_variable_definition_callback, 1},
    {"context_set_variable_assignment_callback", (DL_FUNC) &r_context_set_variable_assignment_callback, 2},
    {"context_get_variable_assignment_callback", (DL_FUNC) &r_context_get_variable_assignment_callback, 1},
    {"context_has_variable_assignment_callback", (DL_FUNC) &r_context_has_variable_assignment_callback, 1},
    {"context_set_variable_removal_callback", (DL_FUNC) &r_context_set_variable_removal_callback, 2},
    {"context_get_variable_removal_callback", (DL_FUNC) &r_context_get_variable_removal_callback, 1},
    {"context_has_variable_removal_callback", (DL_FUNC) &r_context_has_variable_removal_callback, 1},
    {"context_set_variable_lookup_callback", (DL_FUNC) &r_context_set_variable_lookup_callback, 2},
    {"context_get_variable_lookup_callback", (DL_FUNC) &r_context_get_variable_lookup_callback, 1},
    {"context_has_variable_lookup_callback", (DL_FUNC) &r_context_has_variable_lookup_callback, 1},
    {"context_set_environment", (DL_FUNC) &r_context_set_environment, 2},
    {"context_get_environment", (DL_FUNC) &r_context_get_environment, 1},
    {"context_trace_package", (DL_FUNC) &r_context_trace_package, 2},
    {"context_trace_function", (DL_FUNC) &r_context_trace_function, 3},
    {"context_get_traced_packages", (DL_FUNC) &r_context_get_traced_packages, 1},
    {"context_get_traced_functions", (DL_FUNC) &r_context_get_traced_functions, 2},
    {"context_is_package_traced", (DL_FUNC) &r_context_is_package_traced, 2},
    {"context_is_function_traced", (DL_FUNC) &r_context_is_function_traced, 3},
    {"context_is_tracing_enabled", (DL_FUNC) &r_context_is_tracing_enabled, 1},
    {"context_disable_tracing", (DL_FUNC) &r_context_disable_tracing, 1},
    {"context_enable_tracing", (DL_FUNC) &r_context_enable_tracing, 1},
    {"context_reinstate_tracing", (DL_FUNC) &r_context_reinstate_tracing, 1},
    {"context_initialize_tracing", (DL_FUNC) &r_context_initialize_tracing, 1},
    {"context_finalize_tracing", (DL_FUNC) &r_context_finalize_tracing, 1},
    {"context_get_current_callback_type", (DL_FUNC) &r_context_get_current_callback_type, 1},
    {"context_trace_code", (DL_FUNC) &r_context_trace_code, 3},
    {"context_trace_application_load", (DL_FUNC) &r_context_trace_application_load, 2},
    {"context_trace_application_unload", (DL_FUNC) &r_context_trace_application_unload, 2},
    {"context_trace_application_attach", (DL_FUNC) &r_context_trace_application_attach, 2},
    {"context_trace_application_detach", (DL_FUNC) &r_context_trace_application_detach, 2},
    {"context_trace_package_load", (DL_FUNC) &r_context_trace_package_load, 3},
    {"context_trace_package_unload", (DL_FUNC) &r_context_trace_package_unload, 3},
    {"context_trace_package_attach", (DL_FUNC) &r_context_trace_package_attach, 3},
    {"context_trace_package_detach", (DL_FUNC) &r_context_trace_package_detach, 3},
    {"context_trace_function_attach", (DL_FUNC) &r_context_trace_function_attach, 4},
    {"context_trace_function_detach", (DL_FUNC) &r_context_trace_function_detach, 4},
    {"context_trace_call_entry", (DL_FUNC) &r_context_trace_call_entry, 5},
    {"context_trace_call_exit", (DL_FUNC) &r_context_trace_call_exit, 5},

    /* Application */
    {"application_create_application", (DL_FUNC) &r_application_create_application, 5},
    {"application_get_name", (DL_FUNC) &r_application_get_name, 1},
    {"application_get_directory", (DL_FUNC) &r_application_get_directory, 1},
    {"application_get_code", (DL_FUNC) &r_application_get_code, 1},
    {"application_get_environment", (DL_FUNC) &r_application_get_environment, 1},
    {"application_get_frame_position", (DL_FUNC) &r_application_get_frame_position, 1},
    {"application_get_packages", (DL_FUNC) &r_application_get_packages, 1},
    {"application_get_call_stack", (DL_FUNC) &r_application_get_call_stack, 1},
    {"application_add_package", (DL_FUNC) &r_application_add_package, 2},

    /* Package */
    {"package_create_package", (DL_FUNC) &r_package_create_package, 3},
    {"package_get_name", (DL_FUNC) &r_package_get_name, 1},
    {"package_get_directory", (DL_FUNC) &r_package_get_directory, 1},
    {"package_get_environment", (DL_FUNC) &r_package_get_environment, 1},
    {"package_get_functions", (DL_FUNC) &r_package_get_functions, 1},
    {"package_add_function", (DL_FUNC) &r_package_add_function, 2},

    /* Function */
    {"function_create_function", (DL_FUNC) &r_function_create_function, 6},
    {"function_get_name", (DL_FUNC) &r_function_get_name, 1},
    {"function_get_parameter_count", (DL_FUNC) &r_function_get_parameter_count, 1},
    {"function_get_definition", (DL_FUNC) &r_function_get_definition, 1},
    {"function_is_public", (DL_FUNC) &r_function_is_public, 1},
    {"function_is_s3_generic", (DL_FUNC) &r_function_is_s3_generic, 1},
    {"function_is_s3_method", (DL_FUNC) &r_function_is_s3_method, 1},
    {"function_get_default_argument_by_position", (DL_FUNC) &r_function_get_default_argument_by_position, 2},
    {"function_get_default_argument_by_name", (DL_FUNC) &r_function_get_default_argument_by_name, 2},

    /* Call */
    {"call_create_call", (DL_FUNC) &r_call_create_call, 4},
    {"call_get_function", (DL_FUNC) &r_call_get_function, 1},
    {"call_get_expression", (DL_FUNC) &r_call_get_expression, 1},
    {"call_get_environment", (DL_FUNC) &r_call_get_environment, 1},
    {"call_get_frame_position", (DL_FUNC) &r_call_get_frame_position, 1},
    {"call_is_active", (DL_FUNC) &r_call_is_active, 1},
    {"call_is_successful", (DL_FUNC) &r_call_is_successful, 1},
    {"call_get_result", (DL_FUNC) &r_call_get_result, 1},
    {"call_get_parameters", (DL_FUNC) &r_call_get_parameters, 1},
    {"call_get_parameter_by_name", (DL_FUNC) &r_call_get_parameter_by_name, 2},
    {"call_get_parameter_by_position", (DL_FUNC) &r_call_get_parameter_by_position, 2},

    /* Parameter */
    {"parameter_get_name", (DL_FUNC) &r_parameter_get_name, 1},
    {"parameter_get_position", (DL_FUNC) &r_parameter_get_position, 1},
    {"parameter_is_missing", (DL_FUNC) &r_parameter_is_missing, 1},
    {"parameter_is_vararg", (DL_FUNC) &r_parameter_is_vararg, 1},
    {"parameter_get_arguments", (DL_FUNC) &r_parameter_get_arguments, 1},
    {"parameter_get_argument_by_name", (DL_FUNC) &r_parameter_get_argument_by_name, 2},
    {"parameter_get_argument_by_position", (DL_FUNC) &r_parameter_get_argument_by_position, 2},

    /* Argument */
    {"argument_get_name", (DL_FUNC) &r_argument_get_name, 1},
    {"argument_is_evaluated", (DL_FUNC) &r_argument_is_evaluated, 1},
    {"argument_get_expression", (DL_FUNC) &r_argument_get_expression, 1},
    {"argument_get_result", (DL_FUNC) &r_argument_get_result, 1},

    /* CallStack */
    {"call_stack_get_size", (DL_FUNC) &r_call_stack_get_size, 1},
    {"call_stack_peek_frame", (DL_FUNC) &r_call_stack_peek_frame, 2},

    /* Callback */
    {"callback_is_r_callback", (DL_FUNC) &r_callback_is_r_callback, 1},
    {"callback_is_c_callback", (DL_FUNC) &r_callback_is_c_callback, 1},
    {"callback_get_function", (DL_FUNC) &r_callback_get_function, 1},
    {"callback_activate", (DL_FUNC) &r_callback_activate, 1},
    {"callback_deactivate", (DL_FUNC) &r_callback_deactivate, 1},
    {"callback_reinstate", (DL_FUNC) &r_callback_reinstate, 1},
    {"callback_is_active", (DL_FUNC) &r_callback_is_active, 1},

    /* ApplicationAttachCallback */
    {"application_attach_callback_create_from_r_function", (DL_FUNC) &r_application_attach_callback_create_from_r_function, 1},
    {"application_attach_callback_create_from_c_function", (DL_FUNC) &r_application_attach_callback_create_from_c_function, 1},

    /* ApplicationDetachCallback */
    {"application_detach_callback_create_from_r_function", (DL_FUNC) &r_application_detach_callback_create_from_r_function, 1},
    {"application_detach_callback_create_from_c_function", (DL_FUNC) &r_application_detach_callback_create_from_c_function, 1},

    /* ApplicationLoadCallback */
    {"application_load_callback_create_from_r_function", (DL_FUNC) &r_application_load_callback_create_from_r_function, 1},
    {"application_load_callback_create_from_c_function", (DL_FUNC) &r_application_load_callback_create_from_c_function, 1},

    /* ApplicationUnloadCallback */
    {"application_unload_callback_create_from_r_function", (DL_FUNC) &r_application_unload_callback_create_from_r_function, 1},
    {"application_unload_callback_create_from_c_function", (DL_FUNC) &r_application_unload_callback_create_from_c_function, 1},

    /* PackageAttachCallback */
    {"package_attach_callback_create_from_r_function", (DL_FUNC) &r_package_attach_callback_create_from_r_function, 1},
    {"package_attach_callback_create_from_c_function", (DL_FUNC) &r_package_attach_callback_create_from_c_function, 1},

    /* PackageDetachCallback */
    {"package_detach_callback_create_from_r_function", (DL_FUNC) &r_package_detach_callback_create_from_r_function, 1},
    {"package_detach_callback_create_from_c_function", (DL_FUNC) &r_package_detach_callback_create_from_c_function, 1},

    /* PackageLoadCallback */
    {"package_load_callback_create_from_r_function", (DL_FUNC) &r_package_load_callback_create_from_r_function, 1},
    {"package_load_callback_create_from_c_function", (DL_FUNC) &r_package_load_callback_create_from_c_function, 1},

    /* PackageUnloadCallback */
    {"package_unload_callback_create_from_r_function", (DL_FUNC) &r_package_unload_callback_create_from_r_function, 1},
    {"package_unload_callback_create_from_c_function", (DL_FUNC) &r_package_unload_callback_create_from_c_function, 1},

    /* FunctionAttachCallback */
    {"function_attach_callback_create_from_r_function", (DL_FUNC) &r_function_attach_callback_create_from_r_function, 1},
    {"function_attach_callback_create_from_c_function", (DL_FUNC) &r_function_attach_callback_create_from_c_function, 1},

    /* FunctionDetachCallback */
    {"function_detach_callback_create_from_r_function", (DL_FUNC) &r_function_detach_callback_create_from_r_function, 1},
    {"function_detach_callback_create_from_c_function", (DL_FUNC) &r_function_detach_callback_create_from_c_function, 1},

    /* CallEntryCallback */
    {"call_entry_callback_create_from_r_function", (DL_FUNC) &r_call_entry_callback_create_from_r_function, 1},
    {"call_entry_callback_create_from_c_function", (DL_FUNC) &r_call_entry_callback_create_from_c_function, 1},

    /* CallExitCallback */
    {"call_exit_callback_create_from_r_function", (DL_FUNC) &r_call_exit_callback_create_from_r_function, 1},
    {"call_exit_callback_create_from_c_function", (DL_FUNC) &r_call_exit_callback_create_from_c_function, 1},

    /* ObjectCoerceCallback */
    {"object_coerce_callback_create_from_r_function", (DL_FUNC) &r_object_coerce_callback_create_from_r_function, 1},
    {"object_coerce_callback_create_from_c_function", (DL_FUNC) &r_object_coerce_callback_create_from_c_function, 1},

    /* ObjectDuplicateCallback */
    {"object_duplicate_callback_create_from_r_function", (DL_FUNC) &r_object_duplicate_callback_create_from_r_function, 1},
    {"object_duplicate_callback_create_from_c_function", (DL_FUNC) &r_object_duplicate_callback_create_from_c_function, 1},

    /* VectorCopyCallback */
    {"vector_copy_callback_create_from_r_function", (DL_FUNC) &r_vector_copy_callback_create_from_r_function, 1},
    {"vector_copy_callback_create_from_c_function", (DL_FUNC) &r_vector_copy_callback_create_from_c_function, 1},

    /* MatrixCopyCallback */
    {"matrix_copy_callback_create_from_r_function", (DL_FUNC) &r_matrix_copy_callback_create_from_r_function, 1},
    {"matrix_copy_callback_create_from_c_function", (DL_FUNC) &r_matrix_copy_callback_create_from_c_function, 1},

    /* BuiltinCallEntryCallback */
    {"builtin_call_entry_callback_create_from_r_function", (DL_FUNC) &r_builtin_call_entry_callback_create_from_r_function, 1},
    {"builtin_call_entry_callback_create_from_c_function", (DL_FUNC) &r_builtin_call_entry_callback_create_from_c_function, 1},

    /* BuiltinCallExitCallback */
    {"builtin_call_exit_callback_create_from_r_function", (DL_FUNC) &r_builtin_call_exit_callback_create_from_r_function, 1},
    {"builtin_call_exit_callback_create_from_c_function", (DL_FUNC) &r_builtin_call_exit_callback_create_from_c_function, 1},

    /* SpecialCallEntryCallback */
    {"special_call_entry_callback_create_from_r_function", (DL_FUNC) &r_special_call_entry_callback_create_from_r_function, 1},
    {"special_call_entry_callback_create_from_c_function", (DL_FUNC) &r_special_call_entry_callback_create_from_c_function, 1},

    /* SpecialCallExitCallback */
    {"special_call_exit_callback_create_from_r_function", (DL_FUNC) &r_special_call_exit_callback_create_from_r_function, 1},
    {"special_call_exit_callback_create_from_c_function", (DL_FUNC) &r_special_call_exit_callback_create_from_c_function, 1},

    /* ClosureCallEntryCallback */
    {"closure_call_entry_callback_create_from_r_function", (DL_FUNC) &r_closure_call_entry_callback_create_from_r_function, 1},
    {"closure_call_entry_callback_create_from_c_function", (DL_FUNC) &r_closure_call_entry_callback_create_from_c_function, 1},

    /* ClosureCallExitCallback */
    {"closure_call_exit_callback_create_from_r_function", (DL_FUNC) &r_closure_call_exit_callback_create_from_r_function, 1},
    {"closure_call_exit_callback_create_from_c_function", (DL_FUNC) &r_closure_call_exit_callback_create_from_c_function, 1},

    /* EvalEntryCallback */
    {"eval_entry_callback_create_from_r_function", (DL_FUNC) &r_eval_entry_callback_create_from_r_function, 1},
    {"eval_entry_callback_create_from_c_function", (DL_FUNC) &r_eval_entry_callback_create_from_c_function, 1},

    /* EvalExitCallback */
    {"eval_exit_callback_create_from_r_function", (DL_FUNC) &r_eval_exit_callback_create_from_r_function, 1},
    {"eval_exit_callback_create_from_c_function", (DL_FUNC) &r_eval_exit_callback_create_from_c_function, 1},

    /* GcAllocationCallback */
    {"gc_allocation_callback_create_from_r_function", (DL_FUNC) &r_gc_allocation_callback_create_from_r_function, 1},
    {"gc_allocation_callback_create_from_c_function", (DL_FUNC) &r_gc_allocation_callback_create_from_c_function, 1},

    /* GcUnmarkCallback */
    {"gc_unmark_callback_create_from_r_function", (DL_FUNC) &r_gc_unmark_callback_create_from_r_function, 1},
    {"gc_unmark_callback_create_from_c_function", (DL_FUNC) &r_gc_unmark_callback_create_from_c_function, 1},

    /* VariableDefinitionCallback */
    {"variable_definition_callback_create_from_r_function", (DL_FUNC) &r_variable_definition_callback_create_from_r_function, 1},
    {"variable_definition_callback_create_from_c_function", (DL_FUNC) &r_variable_definition_callback_create_from_c_function, 1},

    /* VariableAssignmentCallback */
    {"variable_assignment_callback_create_from_r_function", (DL_FUNC) &r_variable_assignment_callback_create_from_r_function, 1},
    {"variable_assignment_callback_create_from_c_function", (DL_FUNC) &r_variable_assignment_callback_create_from_c_function, 1},

    /* VariableRemovalCallback */
    {"variable_removal_callback_create_from_r_function", (DL_FUNC) &r_variable_removal_callback_create_from_r_function, 1},
    {"variable_removal_callback_create_from_c_function", (DL_FUNC) &r_variable_removal_callback_create_from_c_function, 1},

    /* VariableLookupCallback */
    {"variable_lookup_callback_create_from_r_function", (DL_FUNC) &r_variable_lookup_callback_create_from_r_function, 1},
    {"variable_lookup_callback_create_from_c_function", (DL_FUNC) &r_variable_lookup_callback_create_from_c_function, 1},

    {NULL, NULL, 0}
};

void R_init_instrumentr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
