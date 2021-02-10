#include <instrumentr/api.h>
#include "instrumentr_internals.h"
#include "trace.h"
#include "object.h"
#include "tracer.h"
#include "application.h"
#include "package.h"
#include "function.h"
#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "call_stack.h"
#include "callback.h"
#include "context.h"
#include "interop.h"

#include <R_ext/Rdynload.h>

#include <stdio.h>

#define DECLARE_BINDING(NAME, PARAMETERS) {#NAME, (DL_FUNC) &r_##NAME, PARAMETERS}

const R_CallMethodDef CallEntries[] = {

    /* instrumentr */
    DECLARE_BINDING(instrumentr_get_commit_hash, 0),
    DECLARE_BINDING(instrumentr_is_tracing_enabled, 0),
    DECLARE_BINDING(instrumentr_disable_tracing, 0),
    DECLARE_BINDING(instrumentr_enable_tracing, 0),
    DECLARE_BINDING(instrumentr_reinstate_tracing, 0),
    DECLARE_BINDING(instrumentr_initialize, 2),
    DECLARE_BINDING(instrumentr_finalize, 0),
    DECLARE_BINDING(instrumentr_initialize_tracing, 1),
    DECLARE_BINDING(instrumentr_finalize_tracing, 1),
    DECLARE_BINDING(instrumentr_get_object_details, 4),

    /* trace */
    DECLARE_BINDING(instrumentr_trace_code, 3),
    DECLARE_BINDING(instrumentr_trace_tracing_initialization, 2),
    DECLARE_BINDING(instrumentr_trace_tracing_finalization, 2),
    DECLARE_BINDING(instrumentr_trace_package_load, 3),
    DECLARE_BINDING(instrumentr_trace_package_unload, 3),
    DECLARE_BINDING(instrumentr_trace_package_attach, 3),
    DECLARE_BINDING(instrumentr_trace_package_detach, 3),

    /* object */
    DECLARE_BINDING(instrumentr_object_get_id, 1),
    DECLARE_BINDING(instrumentr_object_has_data, 1),
    DECLARE_BINDING(instrumentr_object_get_data, 1),
    DECLARE_BINDING(instrumentr_object_set_data, 2),
    DECLARE_BINDING(instrumentr_object_remove_data, 1),

    /* tracer */
    DECLARE_BINDING(instrumentr_tracer_create, 0),
    DECLARE_BINDING(instrumentr_tracer_get_application, 1),
    DECLARE_BINDING(instrumentr_tracer_get_environment, 1),
    DECLARE_BINDING(instrumentr_tracer_is_active, 1),
    DECLARE_BINDING(instrumentr_tracer_get_active_callback, 1),
    DECLARE_BINDING(instrumentr_tracer_is_enabled, 1),
    DECLARE_BINDING(instrumentr_tracer_enable, 1),
    DECLARE_BINDING(instrumentr_tracer_disable, 1),
    DECLARE_BINDING(instrumentr_tracer_reinstate, 1),
    DECLARE_BINDING(instrumentr_tracer_has_callback, 2),
    DECLARE_BINDING(instrumentr_tracer_get_callback, 2),
    DECLARE_BINDING(instrumentr_tracer_set_callback, 2),
    DECLARE_BINDING(instrumentr_tracer_remove_callback, 2),
    DECLARE_BINDING(instrumentr_tracer_get_tracing_exec_stats, 1),
    DECLARE_BINDING(instrumentr_tracer_get_event_exec_stats, 2),
    DECLARE_BINDING(instrumentr_tracer_get_exec_stats, 1),

    /* callback */
    DECLARE_BINDING(instrumentr_callback_create_from_r_function, 2),
    DECLARE_BINDING(instrumentr_callback_create_from_c_function, 2),
    DECLARE_BINDING(instrumentr_callback_get_event, 1),
    DECLARE_BINDING(instrumentr_callback_handles_event, 2),
    DECLARE_BINDING(instrumentr_callback_get_parameter_count, 1),
    DECLARE_BINDING(instrumentr_callback_has_r_function, 1),
    DECLARE_BINDING(instrumentr_callback_get_r_function, 1),
    DECLARE_BINDING(instrumentr_callback_has_c_function, 1),
    DECLARE_BINDING(instrumentr_callback_get_c_function, 1),
    DECLARE_BINDING(instrumentr_callback_is_active, 1),
    DECLARE_BINDING(instrumentr_callback_get_exec_stats, 1),

    /* application */
    DECLARE_BINDING(instrumentr_application_create, 5),
    DECLARE_BINDING(instrumentr_application_get_name, 1),
    DECLARE_BINDING(instrumentr_application_get_directory, 1),
    DECLARE_BINDING(instrumentr_application_get_code, 1),
    DECLARE_BINDING(instrumentr_application_get_environment, 1),
    DECLARE_BINDING(instrumentr_application_get_frame_position, 1),
    DECLARE_BINDING(instrumentr_application_get_package_count, 1),
    DECLARE_BINDING(instrumentr_application_get_package_by_position, 2),
    DECLARE_BINDING(instrumentr_application_get_package_by_name, 2),
    DECLARE_BINDING(instrumentr_application_get_packages, 1),
    DECLARE_BINDING(instrumentr_application_add_package, 2),
    DECLARE_BINDING(instrumentr_application_remove_package, 2),
    DECLARE_BINDING(instrumentr_application_get_call_stack, 1),
    DECLARE_BINDING(instrumentr_application_function_map_update_properties, 8),

    /* call_stack */
    DECLARE_BINDING(instrumentr_call_stack_get_size, 1),
    DECLARE_BINDING(instrumentr_call_stack_peek_frame, 2),

    /* package */
    DECLARE_BINDING(instrumentr_package_create, 4),
    DECLARE_BINDING(instrumentr_package_add_function, 2),
    DECLARE_BINDING(instrumentr_package_get_name, 1),
    DECLARE_BINDING(instrumentr_package_get_directory, 1),
    DECLARE_BINDING(instrumentr_package_get_namespace, 1),
    DECLARE_BINDING(instrumentr_package_is_attached, 1),
    DECLARE_BINDING(instrumentr_package_get_function_count, 1),
    DECLARE_BINDING(instrumentr_package_has_function, 2),
    DECLARE_BINDING(instrumentr_package_get_function_by_name, 2),
    DECLARE_BINDING(instrumentr_package_get_function_by_position, 2),
    DECLARE_BINDING(instrumentr_package_get_functions, 1),

    /* function */
    DECLARE_BINDING(instrumentr_function_has_name, 1),
    DECLARE_BINDING(instrumentr_function_get_name, 1),
    DECLARE_BINDING(instrumentr_function_get_parameter_count, 1),
    DECLARE_BINDING(instrumentr_function_get_definition, 1),
    DECLARE_BINDING(instrumentr_function_is_inner, 1),
    DECLARE_BINDING(instrumentr_function_get_parent, 1),
    DECLARE_BINDING(instrumentr_function_is_public, 1),
    DECLARE_BINDING(instrumentr_function_is_s3_generic, 1),
    DECLARE_BINDING(instrumentr_function_is_s3_method, 1),

    /* call */
    DECLARE_BINDING(instrumentr_call_get_function, 1),
    DECLARE_BINDING(instrumentr_call_get_expression, 1),
    DECLARE_BINDING(instrumentr_call_get_environment, 1),
    DECLARE_BINDING(instrumentr_call_get_frame_position, 1),
    DECLARE_BINDING(instrumentr_call_is_active, 1),
    DECLARE_BINDING(instrumentr_call_has_result, 1),
    DECLARE_BINDING(instrumentr_call_get_result, 1),
    DECLARE_BINDING(instrumentr_call_get_parameter_count, 1),
    DECLARE_BINDING(instrumentr_call_get_parameter_by_name, 2),
    DECLARE_BINDING(instrumentr_call_get_parameter_by_position, 2),
    DECLARE_BINDING(instrumentr_call_get_parameters, 1),

    /* parameter */
    DECLARE_BINDING(instrumentr_parameter_get_name, 1),
    DECLARE_BINDING(instrumentr_parameter_get_position, 1),
    DECLARE_BINDING(instrumentr_parameter_is_missing, 1),
    DECLARE_BINDING(instrumentr_parameter_is_vararg, 1),
    DECLARE_BINDING(instrumentr_parameter_get_argument_count, 1),
    DECLARE_BINDING(instrumentr_parameter_get_argument_by_name, 2),
    DECLARE_BINDING(instrumentr_parameter_get_argument_by_position, 2),
    DECLARE_BINDING(instrumentr_parameter_get_arguments, 1),
    DECLARE_BINDING(instrumentr_parameter_has_default_argument, 1),
    DECLARE_BINDING(instrumentr_parameter_get_default_argument, 1),

    /* argument */
    DECLARE_BINDING(instrumentr_argument_has_name, 1),
    DECLARE_BINDING(instrumentr_argument_get_name, 1),
    DECLARE_BINDING(instrumentr_argument_is_value, 1),
    DECLARE_BINDING(instrumentr_argument_as_value, 1),
    DECLARE_BINDING(instrumentr_argument_is_promise, 1),
    DECLARE_BINDING(instrumentr_argument_as_promise, 1),

    /* promise */
    DECLARE_BINDING(instrumentr_promise_is_forced, 1),
    DECLARE_BINDING(instrumentr_promise_get_expression, 1),
    DECLARE_BINDING(instrumentr_promise_get_value, 1),

    /* value */
    DECLARE_BINDING(instrumentr_value_get_sexp, 1),
    DECLARE_BINDING(instrumentr_value_get_sexp_address, 1),
    DECLARE_BINDING(instrumentr_value_get_sexp_type, 1),

    /* context */
    DECLARE_BINDING(instrumentr_context_get_pointer, 1),

    /* exec_stats */
    DECLARE_BINDING(instrumentr_exec_stats_get_minimum_time, 1),
    DECLARE_BINDING(instrumentr_exec_stats_get_maximum_time, 1),
    DECLARE_BINDING(instrumentr_exec_stats_get_average_time, 1),
    DECLARE_BINDING(instrumentr_exec_stats_get_total_time, 1),
    DECLARE_BINDING(instrumentr_exec_stats_get_execution_count, 1),
    DECLARE_BINDING(instrumentr_exec_stats_as_data_frame, 1),

    {NULL, NULL, 0}};

void R_init_instrumentr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    INSTRUMENTR_EXPORT_API()
}

void R_unload_instrumentr(DllInfo* info) {
}
