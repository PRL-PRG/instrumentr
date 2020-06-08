#include "r_api.h"
#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* lightr */
    {"lightr_is_tracing_enabled", (DL_FUNC) &r_lightr_is_tracing_enabled, 0},
    {"lightr_disable_tracing", (DL_FUNC) &r_lightr_disable_tracing, 0},
    {"lightr_enable_tracing", (DL_FUNC) &r_lightr_enable_tracing, 0},
    {"lightr_reinstate_tracing", (DL_FUNC) &r_lightr_reinstate_tracing, 0},
    {"lightr_trace_code", (DL_FUNC) &r_lightr_trace_code, 2},
    {"lightr_initialize_lightr", (DL_FUNC) &r_lightr_initialize_lightr, 3},
    {"lightr_initialize_tracing", (DL_FUNC) &r_lightr_initialize_tracing, 0},
    {"lightr_finalize_tracing", (DL_FUNC) &r_lightr_finalize_tracing, 0},
    {"lightr_peek_execution_context", (DL_FUNC) &r_lightr_peek_execution_context, 0},
    {"lightr_trace_application_load", (DL_FUNC) &r_lightr_trace_application_load, 2},
    {"lightr_trace_application_unload", (DL_FUNC) &r_lightr_trace_application_unload, 2},
    {"lightr_trace_application_attach", (DL_FUNC) &r_lightr_trace_application_attach, 2},
    {"lightr_trace_application_detach", (DL_FUNC) &r_lightr_trace_application_detach, 2},
    {"lightr_trace_package_load", (DL_FUNC) &r_lightr_trace_package_load, 3},
    {"lightr_trace_package_unload", (DL_FUNC) &r_lightr_trace_package_unload, 3},
    {"lightr_trace_package_attach", (DL_FUNC) &r_lightr_trace_package_attach, 3},
    {"lightr_trace_package_detach", (DL_FUNC) &r_lightr_trace_package_detach, 3},
    {"lightr_trace_function_attach", (DL_FUNC) &r_lightr_trace_function_attach, 4},
    {"lightr_trace_function_detach", (DL_FUNC) &r_lightr_trace_function_detach, 4},
    {"lightr_trace_call_entry", (DL_FUNC) &r_lightr_trace_call_entry, 5},
    {"lightr_trace_call_exit", (DL_FUNC) &r_lightr_trace_call_exit, 5},

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
    {"context_set_environment", (DL_FUNC) &r_context_set_environment, 2},
    {"context_get_environment", (DL_FUNC) &r_context_get_environment, 1},
    {"context_trace_package", (DL_FUNC) &r_context_trace_package, 2},
    {"context_trace_function", (DL_FUNC) &r_context_trace_function, 3},
    {"context_get_traced_packages", (DL_FUNC) &r_context_get_traced_packages, 1},
    {"context_get_traced_functions", (DL_FUNC) &r_context_get_traced_functions, 2},
    {"context_is_package_traced", (DL_FUNC) &r_context_is_package_traced, 2},
    {"context_is_function_traced", (DL_FUNC) &r_context_is_function_traced, 3},

    /* Application */
    {"application_create_application", (DL_FUNC) &r_application_create_application, 4},
    {"application_get_name", (DL_FUNC) &r_application_get_name, 1},
    {"application_get_directory", (DL_FUNC) &r_application_get_directory, 1},
    {"application_get_code", (DL_FUNC) &r_application_get_code, 1},
    {"application_get_environment", (DL_FUNC) &r_application_get_environment, 1},
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
    {"function_create_function", (DL_FUNC) &r_function_create_function, 3},
    {"function_get_name", (DL_FUNC) &r_function_get_name, 1},
    {"function_get_parameter_count", (DL_FUNC) &r_function_get_parameter_count, 1},
    {"function_get_definition", (DL_FUNC) &r_function_get_definition, 1},

    /* Call */
    {"call_create_call", (DL_FUNC) &r_call_create_call, 3},
    {"call_get_function", (DL_FUNC) &r_call_get_function, 1},
    {"call_get_expression", (DL_FUNC) &r_call_get_expression, 1},
    {"call_get_environment", (DL_FUNC) &r_call_get_environment, 1},
    {"call_is_active", (DL_FUNC) &r_call_is_active, 1},
    {"call_is_successful", (DL_FUNC) &r_call_is_successful, 1},
    {"call_get_result", (DL_FUNC) &r_call_get_result, 1},
    {"call_get_parameters", (DL_FUNC) &r_call_get_parameters, 1},

    /* Parameter */
    {"parameter_get_name", (DL_FUNC) &r_parameter_get_name, 1},
    {"parameter_get_position", (DL_FUNC) &r_parameter_get_position, 1},
    {"parameter_is_missing", (DL_FUNC) &r_parameter_is_missing, 1},
    {"parameter_is_vararg", (DL_FUNC) &r_parameter_is_vararg, 1},
    {"parameter_get_arguments", (DL_FUNC) &r_parameter_get_arguments, 1},

    /* Argument */
    {"argument_get_name", (DL_FUNC) &r_argument_get_name, 1},
    {"argument_is_evaluated", (DL_FUNC) &r_argument_is_evaluated, 1},
    {"argument_get_expression", (DL_FUNC) &r_argument_get_expression, 1},
    {"argument_get_result", (DL_FUNC) &r_argument_get_result, 1},

    /* CallStack */
    {"call_stack_get_size", (DL_FUNC) &r_call_stack_get_size, 1},
    {"call_stack_peek_frame", (DL_FUNC) &r_call_stack_peek_frame, 2},

    {NULL, NULL, 0}};

void R_init_lightr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
