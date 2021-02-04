#include "dyntracer.h"
#include "dyntrace.h"
#include "interop.h"

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer) {
    dyntracer_t* dyntracer = dyntracer_create(NULL);


    dyntracer_set_builtin_entry_callback(dyntracer, dyntrace_basic_call_entry);
    dyntracer_set_builtin_exit_callback(dyntracer, dyntrace_basic_call_exit);
    dyntracer_set_special_entry_callback(dyntracer, dyntrace_basic_call_entry);
    dyntracer_set_special_exit_callback(dyntracer, dyntrace_basic_call_exit);
    dyntracer_set_closure_entry_callback(dyntracer, dyntrace_closure_call_entry);
    dyntracer_set_closure_exit_callback(dyntracer, dyntrace_closure_call_exit);
    dyntracer_set_context_entry_callback(dyntracer, dyntrace_context_entry);
    dyntracer_set_context_exit_callback(dyntracer, dyntrace_context_exit);
    //dyntracer_set_context_jump_callback(dyntracer, context, return_value, restart);
    dyntracer_set_eval_entry_callback(dyntracer, dyntrace_eval_entry);
    dyntracer_set_eval_exit_callback(dyntracer, dyntrace_eval_exit);
    dyntracer_set_gc_allocate_callback(dyntracer, dyntrace_gc_allocation);
    dyntracer_set_environment_variable_define_callback(
        dyntracer, dyntrace_variable_definition);
    dyntracer_set_environment_variable_assign_callback(
        dyntracer, dyntrace_variable_assignment);
    dyntracer_set_environment_variable_remove_callback(
        dyntracer, dyntrace_variable_removal);
    dyntracer_set_environment_variable_lookup_callback(
        dyntracer, dyntrace_variable_lookup);

    dyntracer_set_data(dyntracer, (void*) (tracer));

    return dyntracer;
}

void instrumentr_dyntracer_destroy(dyntracer_t* dyntracer) {
    dyntracer_destroy(dyntracer);
}

instrumentr_tracer_t instrumentr_dyntracer_get_tracer(dyntracer_t* dyntracer) {
    instrumentr_tracer_t tracer =
        (instrumentr_tracer_t)(dyntracer_get_data(dyntracer));

    if (tracer == NULL) {
        instrumentr_log_error(
            "dyntracer has a NULL reference to instrumentr tracer");
    }

    return tracer;
}

