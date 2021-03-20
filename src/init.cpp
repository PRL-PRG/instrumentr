#include <instrumentr/api.h>
#include "instrumentr_internals.h"
#include "trace.h"
#include "object.h"
#include "tracer.h"
#include "application.h"
#include "environment.h"
#include "function.h"
#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "call_stack.h"
#include "callback.h"
#include "context.h"
#include "interop.h"
#include "state.h"
#include "promise.h"
#include "value.h"
#include "exec_stats.h"
#include "frame.h"

#include <R_ext/Rdynload.h>

#include <stdio.h>

#define DECLARE_BINDING(NAME, PARAMETERS) \
    { #NAME, (DL_FUNC) &r_##NAME, PARAMETERS }

#ifdef __cplusplus
extern "C" {
#endif

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
    DECLARE_BINDING(instrumentr_trace_tracing_entry, 4),
    DECLARE_BINDING(instrumentr_trace_tracing_exit, 1),
    DECLARE_BINDING(instrumentr_trace_package_load, 2),
    DECLARE_BINDING(instrumentr_trace_package_unload, 2),
    DECLARE_BINDING(instrumentr_trace_package_attach, 2),
    DECLARE_BINDING(instrumentr_trace_package_detach, 2),

    /* model */
    // TODO

    /* tracer */
    DECLARE_BINDING(instrumentr_tracer_create, 0),
    DECLARE_BINDING(instrumentr_tracer_get_state, 1),
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

    /* state */
    DECLARE_BINDING(instrumentr_state_get_exec_stats, 1),
    DECLARE_BINDING(instrumentr_state_is_empty, 1),
    DECLARE_BINDING(instrumentr_state_get_size, 1),
    DECLARE_BINDING(instrumentr_state_clear, 1),
    DECLARE_BINDING(instrumentr_state_has_key, 2),
    DECLARE_BINDING(instrumentr_state_as_list, 1),
    DECLARE_BINDING(instrumentr_state_lookup, 3),
    DECLARE_BINDING(instrumentr_state_insert, 4),
    DECLARE_BINDING(instrumentr_state_erase, 3),
    DECLARE_BINDING(instrumentr_state_get_call_stack, 1),
    DECLARE_BINDING(instrumentr_state_get_packages, 1),
    DECLARE_BINDING(instrumentr_state_get_namespaces, 1),

    /* application */
    DECLARE_BINDING(instrumentr_application_get_directory, 1),
    DECLARE_BINDING(instrumentr_application_get_code, 1),
    DECLARE_BINDING(instrumentr_application_get_environment, 1),

    /* call_stack */
    DECLARE_BINDING(instrumentr_call_stack_get_size, 1),
    DECLARE_BINDING(instrumentr_call_stack_peek_frame, 2),

    // FRAME API: TODO

    /* environment */
    DECLARE_BINDING(instrumentr_environment_get_name, 1),
    DECLARE_BINDING(instrumentr_environment_get_environment, 1),
    DECLARE_BINDING(instrumentr_environment_get_size, 1),
    DECLARE_BINDING(instrumentr_environment_contains, 2),
    DECLARE_BINDING(instrumentr_environment_lookup, 2),
    DECLARE_BINDING(instrumentr_environment_get_bindings, 1),

    /* function */
    DECLARE_BINDING(instrumentr_function_has_name, 1),
    DECLARE_BINDING(instrumentr_function_get_name, 1),
    DECLARE_BINDING(instrumentr_function_get_parameter_count, 1),
    DECLARE_BINDING(instrumentr_function_get_definition, 1),
    DECLARE_BINDING(instrumentr_function_is_inner, 1),
    DECLARE_BINDING(instrumentr_function_get_parent, 1),
    DECLARE_BINDING(instrumentr_function_is_exported, 1),
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
    DECLARE_BINDING(instrumentr_promise_get_type, 1),
    DECLARE_BINDING(instrumentr_promise_is_argument, 1),
    DECLARE_BINDING(instrumentr_promise_is_delayed_assign, 1),
    DECLARE_BINDING(instrumentr_promise_is_lazy_load, 1),
    DECLARE_BINDING(instrumentr_promise_is_unknown, 1),
    DECLARE_BINDING(instrumentr_promise_get_call, 1),

    /* value */
    DECLARE_BINDING(instrumentr_value_get_sexp, 1),
    DECLARE_BINDING(instrumentr_value_get_sexp_address, 1),
    DECLARE_BINDING(instrumentr_value_get_sexp_type, 1),

    /* context */
    DECLARE_BINDING(instrumentr_context_get_pointer, 1),

    /* exec_stats */
    DECLARE_BINDING(instrumentr_exec_stats_get_minimum_time, 2),
    DECLARE_BINDING(instrumentr_exec_stats_get_maximum_time, 2),
    DECLARE_BINDING(instrumentr_exec_stats_get_average_time, 2),
    DECLARE_BINDING(instrumentr_exec_stats_get_total_time, 2),
    DECLARE_BINDING(instrumentr_exec_stats_get_execution_count, 2),

    {NULL, NULL, 0}};

void R_init_instrumentr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    INSTRUMENTR_EXPORT_API()
}

void R_unload_instrumentr(DllInfo* info) {
}

#ifdef __cplusplus
}
#endif
