#include "r_instrumentr.h"
#include "object_internals.h"
#include <instrumentr/object.h>
#include "r_context.h"
#include <instrumentr/application.h>
#include "application_internals.h"
#include <instrumentr/package.h>
#include <instrumentr/function.h>
#include <instrumentr/call.h>
#include "call_internals.h"
#include <instrumentr/parameter.h>
#include <instrumentr/argument.h>
#include <instrumentr/call_stack.h>
#include <instrumentr/callback.h>

#include <R_ext/Rdynload.h>

#include <stdio.h>

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

    /* object */
    {"instrumentr_object_get_id", (DL_FUNC) &r_instrumentr_object_get_id, 1},
    {"instrumentr_object_has_c_data", (DL_FUNC) &r_instrumentr_object_has_c_data, 1},
    {"instrumentr_object_get_c_data", (DL_FUNC) &r_instrumentr_object_get_c_data, 1},
    {"instrumentr_object_set_c_data", (DL_FUNC) &r_instrumentr_object_set_c_data, 2},
    {"instrumentr_object_remove_c_data", (DL_FUNC) &r_instrumentr_object_remove_c_data, 1},
    {"instrumentr_object_has_r_data", (DL_FUNC) &r_instrumentr_object_has_r_data, 1},
    {"instrumentr_object_get_r_data", (DL_FUNC) &r_instrumentr_object_get_r_data, 1},
    {"instrumentr_object_set_r_data", (DL_FUNC) &r_instrumentr_object_set_r_data, 2},
    {"instrumentr_object_remove_r_data", (DL_FUNC) &r_instrumentr_object_remove_r_data, 1},

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

    /* application */
    {"instrumentr_application_create", (DL_FUNC) &r_instrumentr_application_create_application, 5},
    {"instrumentr_application_get_name", (DL_FUNC) &r_instrumentr_application_get_name, 1},
    {"instrumentr_application_get_directory", (DL_FUNC) &r_instrumentr_application_get_directory, 1},
    {"instrumentr_application_get_code", (DL_FUNC) &r_instrumentr_application_get_code, 1},
    {"instrumentr_application_get_environment", (DL_FUNC) &r_instrumentr_application_get_environment, 1},
    {"instrumentr_application_get_frame_position", (DL_FUNC) &r_instrumentr_application_get_frame_position, 1},
    {"instrumentr_application_get_packages", (DL_FUNC) &r_instrumentr_application_get_packages, 1},
    {"instrumentr_application_get_call_stack", (DL_FUNC) &r_instrumentr_application_get_call_stack, 1},
    {"instrumentr_application_add_package", (DL_FUNC) &r_instrumentr_application_add_package, 2},

    /* package */
    {"instrumentr_package_create_package", (DL_FUNC) &r_instrumentr_package_create_package, 3},
    {"instrumentr_package_get_name", (DL_FUNC) &r_instrumentr_package_get_name, 1},
    {"instrumentr_package_get_directory", (DL_FUNC) &r_instrumentr_package_get_directory, 1},
    {"instrumentr_package_get_namespace", (DL_FUNC) &r_instrumentr_package_get_namespace, 1},
    {"instrumentr_package_get_functions", (DL_FUNC) &r_instrumentr_package_get_functions, 1},

    /* function */
    {"instrumentr_function_create_function", (DL_FUNC) &r_instrumentr_function_create_function, 6},
    {"instrumentr_function_get_name", (DL_FUNC) &r_instrumentr_function_get_name, 1},
    {"instrumentr_function_get_parameter_count", (DL_FUNC) &r_instrumentr_function_get_parameter_count, 1},
    {"instrumentr_function_get_definition", (DL_FUNC) &r_instrumentr_function_get_definition, 1},
    {"instrumentr_function_is_public", (DL_FUNC) &r_instrumentr_function_is_public, 1},
    {"instrumentr_function_is_s3_generic", (DL_FUNC) &r_instrumentr_function_is_s3_generic, 1},
    {"instrumentr_function_is_s3_method", (DL_FUNC) &r_instrumentr_function_is_s3_method, 1},

    /* call */
    {"instrumentr_call_create", (DL_FUNC) &r_instrumentr_call_create, 4},
    {"instrumentr_call_get_function", (DL_FUNC) &r_instrumentr_call_get_function, 1},
    {"instrumentr_call_get_expression", (DL_FUNC) &r_instrumentr_call_get_expression, 1},
    {"instrumentr_call_get_environment", (DL_FUNC) &r_instrumentr_call_get_environment, 1},
    {"instrumentr_call_get_frame_position", (DL_FUNC) &r_instrumentr_call_get_frame_position, 1},
    {"instrumentr_call_is_active", (DL_FUNC) &r_instrumentr_call_is_active, 1},
    {"instrumentr_call_has_result", (DL_FUNC) &r_instrumentr_call_has_result, 1},
    {"instrumentr_call_get_result", (DL_FUNC) &r_instrumentr_call_get_result, 1},
    {"instrumentr_call_get_parameter_count", (DL_FUNC) &r_instrumentr_call_get_parameter_count, 1},
    {"instrumentr_call_get_parameter_by_name", (DL_FUNC) &r_instrumentr_call_get_parameter_by_name, 2},
    {"instrumentr_call_get_parameter_by_position", (DL_FUNC) &r_instrumentr_call_get_parameter_by_position, 2},
    {"instrumentr_call_get_parameters", (DL_FUNC) &r_instrumentr_call_get_parameters, 1},

    /* parameter */
    {"instrumentr_parameter_get_name", (DL_FUNC) &r_instrumentr_parameter_get_name, 1},
    {"instrumentr_parameter_get_position", (DL_FUNC) &r_instrumentr_parameter_get_position, 1},
    {"instrumentr_parameter_get_arguments", (DL_FUNC) &r_instrumentr_parameter_get_arguments, 1},
    {"instrumentr_parameter_is_missing", (DL_FUNC) &r_instrumentr_parameter_is_missing, 1},
    {"instrumentr_parameter_is_vararg", (DL_FUNC) &r_instrumentr_parameter_is_vararg, 1},

    /* argument */
    {"instrumentr_argument_has_name", (DL_FUNC) &r_instrumentr_argument_has_name, 1},
    {"instrumentr_argument_get_name", (DL_FUNC) &r_instrumentr_argument_get_name, 1},
    {"instrumentr_argument_is_evaluated", (DL_FUNC) &r_instrumentr_argument_is_evaluated, 1},
    {"instrumentr_argument_get_promise", (DL_FUNC) &r_instrumentr_argument_get_promise, 1},
    {"instrumentr_argument_get_expression", (DL_FUNC) &r_instrumentr_argument_get_expression, 1},
    {"instrumentr_argument_get_value", (DL_FUNC) &r_instrumentr_argument_get_value, 1},

    /* call_stack */
    {"instrumentr_call_stack_get_size", (DL_FUNC) &r_instrumentr_call_stack_get_size, 1},
    {"instrumentr_call_stack_peek_frame", (DL_FUNC) &r_instrumentr_call_stack_peek_frame, 2},

    /* callback */
    {"instrumentr_callback_has_r_function", (DL_FUNC) &r_instrumentr_callback_has_r_function, 1},
    {"instrumentr_callback_has_c_function", (DL_FUNC) &r_instrumentr_callback_has_c_function, 1},
    {"instrumentr_callback_get_r_function", (DL_FUNC) &r_instrumentr_callback_get_r_function, 1},
    {"instrumentr_callback_get_c_function", (DL_FUNC) &r_instrumentr_callback_get_c_function, 1},
    {"instrumentr_callback_is_active", (DL_FUNC) &r_instrumentr_callback_is_active, 1},
    {"instrumentr_callback_is_enabled", (DL_FUNC) &r_instrumentr_callback_is_enabled, 1},
    {"instrumentr_callback_enable", (DL_FUNC) &r_instrumentr_callback_enable, 1},
    {"instrumentr_callback_disable", (DL_FUNC) &r_instrumentr_callback_disable, 1},
    {"instrumentr_callback_reinstate", (DL_FUNC) &r_instrumentr_callback_reinstate, 1},

#define CALLBACK_CREATE(TYPE, NAME)                                                                                                  \
    {"instrumentr_callback_create_" #NAME "_from_r_function", (DL_FUNC) &r_instrumentr_callback_create_##NAME##_from_r_function, 1}, \
    {"instrumentr_callback_create_" #NAME "_from_c_function", (DL_FUNC) &r_instrumentr_callback_create_##NAME##_from_c_function, 1},

    INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(CALLBACK_CREATE)

#undef CALLBACK_CREATE

    {NULL, NULL, 0}
};

void R_init_instrumentr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    instrumentr_object_class_initialize();
}
