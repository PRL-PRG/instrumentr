#include "dyntrace.h"
#include "trace.h"
#include "interop.h"

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer) {
    dyntracer_t* dyntracer = dyntracer_create(NULL);

    dyntracer_set_builtin_entry_callback(dyntracer,
                                         instrumentr_trace_builtin_call_entry);
    dyntracer_set_builtin_exit_callback(dyntracer,
                                        instrumentr_trace_builtin_call_exit);
    dyntracer_set_special_entry_callback(dyntracer,
                                         instrumentr_trace_special_call_entry);
    dyntracer_set_special_exit_callback(dyntracer,
                                        instrumentr_trace_special_call_exit);
    dyntracer_set_closure_entry_callback(dyntracer,
                                         instrumentr_trace_closure_call_entry);
    dyntracer_set_closure_exit_callback(dyntracer,
                                        instrumentr_trace_closure_call_exit);
    dyntracer_set_context_entry_callback(dyntracer,
                                         instrumentr_trace_context_entry);
    dyntracer_set_context_exit_callback(dyntracer,
                                        instrumentr_trace_context_exit);

    dyntracer_set_use_method_entry_callback(dyntracer,
                                            instrumentr_trace_use_method_entry);

    dyntracer_set_use_method_exit_callback(dyntracer,
                                           instrumentr_trace_use_method_exit);

    dyntracer_set_context_jump_callback(dyntracer,
                                        instrumentr_trace_context_jump);
    dyntracer_set_eval_entry_callback(dyntracer, instrumentr_trace_eval_entry);
    dyntracer_set_eval_exit_callback(dyntracer, instrumentr_trace_eval_exit);
    dyntracer_set_gc_allocate_callback(dyntracer,
                                       instrumentr_trace_gc_allocation);
    dyntracer_set_gc_unmark_callback(dyntracer,
                                     instrumentr_trace_gc_deallocation);

    dyntracer_set_promise_force_entry_callback(
        dyntracer, instrumentr_trace_promise_force_entry);

    dyntracer_set_promise_force_exit_callback(
        dyntracer, instrumentr_trace_promise_force_exit);

    dyntracer_set_promise_value_lookup_callback(
        dyntracer, instrumentr_trace_promise_value_lookup);

    dyntracer_set_promise_expression_lookup_callback(
        dyntracer, instrumentr_trace_promise_expression_lookup);

    dyntracer_set_promise_delayed_assign_callback(
        dyntracer, instrumentr_trace_promise_delayed_assign);

    dyntracer_set_promise_lazy_load_callback(
        dyntracer, instrumentr_trace_promise_lazy_load);

    dyntracer_set_promise_substitute_callback(
        dyntracer, instrumentr_trace_promise_substitute);

    dyntracer_set_environment_variable_define_callback(
        dyntracer, instrumentr_trace_variable_definition);

    dyntracer_set_environment_variable_assign_callback(
        dyntracer, instrumentr_trace_variable_assignment);

    dyntracer_set_environment_variable_remove_callback(
        dyntracer, instrumentr_trace_variable_removal);

    dyntracer_set_environment_variable_lookup_callback(
        dyntracer, instrumentr_trace_variable_lookup);

    dyntracer_set_environment_function_context_lookup_callback(
        dyntracer, instrumentr_trace_function_context_lookup);

    dyntracer_set_error_callback(dyntracer, instrumentr_trace_error);

    dyntracer_set_attribute_set_callback(dyntracer,
                                         instrumentr_trace_attribute_set);

    dyntracer_set_data(dyntracer, (void*) (tracer));

    return dyntracer;
}

void instrumentr_dyntracer_destroy(dyntracer_t* dyntracer) {
    dyntracer_destroy(dyntracer);
}

instrumentr_tracer_t instrumentr_dyntracer_get_tracer(dyntracer_t* dyntracer) {
    instrumentr_tracer_t tracer =
        (instrumentr_tracer_t) (dyntracer_get_data(dyntracer));

    if (tracer == NULL) {
        instrumentr_log_error(
            "dyntracer has a NULL reference to instrumentr tracer");
    }

    return tracer;
}
