#include "r_instrumentr.h"
#include "object_internals.h"
#include <instrumentr/object.h>
#include <instrumentr/tracer.h>
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

    /* tracer */
    {"instrumentr_tracer_create", (DL_FUNC) &r_instrumentr_tracer_create, 1},
    {"instrumentr_tracer_get_application", (DL_FUNC) &r_instrumentr_tracer_get_application, 1},
    {"instrumentr_tracer_get_environment", (DL_FUNC) &r_instrumentr_tracer_get_environment, 1},
    {"instrumentr_tracer_is_active", (DL_FUNC) &r_instrumentr_tracer_is_active, 1},
    {"instrumentr_tracer_get_active_callback", (DL_FUNC) &r_instrumentr_get_active_callback, 1},
    {"instrumentr_tracer_is_enabled", (DL_FUNC) &r_instrumentr_tracer_is_enabled, 1},
    {"instrumentr_tracer_enable", (DL_FUNC) &r_instrumentr_tracer_enable, 1},
    {"instrumentr_tracer_disable", (DL_FUNC) &r_instrumentr_tracer_disable, 1},
    {"instrumentr_tracer_reinstate", (DL_FUNC) &r_instrumentr_tracer_reinstate, 1},
    {"instrumentr_tracer_get_traced_package_count", (DL_FUNC) &r_instrumentr_tracer_get_traced_package_count, 1},
    {"instrumentr_tracer_get_traced_function_count", (DL_FUNC) &r_instrumentr_tracer_get_traced_function_count, 2},
    {"instrumentr_tracer_is_package_traced", (DL_FUNC) &r_instrumentr_tracer_is_package_traced, 2},
    {"instrumentr_tracer_is_function_traced", (DL_FUNC) &r_instrumentr_tracer_is_function_traced, 3},
    {"instrumentr_tracer_get_traced_packages", (DL_FUNC) &r_instrumentr_tracer_get_traced_packages, 1},
    {"instrumentr_tracer_get_traced_functions", (DL_FUNC) &r_instrumentr_tracer_get_traced_functions, 2},
    {"instrumentr_tracer_trace_package", (DL_FUNC) &r_instrumentr_tracer_trace_package, 2},
    {"instrumentr_tracer_trace_function", (DL_FUNC) &r_instrumentr_tracer_trace_function, 3},

#define TRACER_CALLBACK_INTERFACE(TYPE, NAME)                                                                     \
    {"instrumentr_tracer_has_" #NAME "_callback", (DL_FUNC) &r_instrumentr_tracer_has_##NAME##_callback, 1},      \
    {"instrumentr_tracer_get_" #NAME "_callback", (DL_FUNC) &r_instrumentr_tracer_get_##NAME##_callback, 1},      \
    {"instrumentr_tracer_set_" #NAME "_callback", (DL_FUNC) &r_instrumentr_tracer_set_##NAME##_callback, 2},      \
    {"instrumentr_tracer_remove_" #NAME "_callback", (DL_FUNC) &r_instrumentr_tracer_remove_##NAME##_callback, 1},

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_INTERFACE)

#undef TRACER_CALLBACK_INTERFACE

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
