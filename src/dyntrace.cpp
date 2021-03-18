#include "dyntrace.h"
#include "interop.h"
#include "call_stack.h"
#include "frame.h"
#include "object.h"
#include "application.h"
#include "trace.h"
#include "funtab.h"
#include "state.h"
#include "package.h"
#include "function.h"
#include "call.h"
#include "context.h"

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer) {
    dyntracer_t* dyntracer = dyntracer_create(NULL);

    dyntracer_set_builtin_entry_callback(dyntracer, dyntrace_basic_call_entry);
    dyntracer_set_builtin_exit_callback(dyntracer, dyntrace_basic_call_exit);
    dyntracer_set_special_entry_callback(dyntracer, dyntrace_basic_call_entry);
    dyntracer_set_special_exit_callback(dyntracer, dyntrace_basic_call_exit);
    dyntracer_set_closure_entry_callback(dyntracer,
                                         dyntrace_closure_call_entry);
    dyntracer_set_closure_exit_callback(dyntracer, dyntrace_closure_call_exit);
    dyntracer_set_context_entry_callback(dyntracer, dyntrace_context_entry);
    dyntracer_set_context_exit_callback(dyntracer, dyntrace_context_exit);
    dyntracer_set_context_jump_callback(dyntracer, dyntrace_context_jump);
    dyntracer_set_eval_entry_callback(dyntracer, dyntrace_eval_entry);
    dyntracer_set_eval_exit_callback(dyntracer, dyntrace_eval_exit);
    dyntracer_set_gc_allocate_callback(dyntracer, dyntrace_gc_allocation);
    dyntracer_set_gc_deallocate_callback(dyntracer, dyntrace_gc_deallocate);
    dyntracer_set_promise_force_entry_callback(dyntracer,
                                               dyntrace_promise_force_entry);
    dyntracer_set_promise_force_exit_callback(dyntracer,
                                              dyntrace_promise_force_exit);

    dyntracer_set_promise_value_lookup_callback(dyntracer,
                                                dyntrace_promise_value_lookup);
    dyntracer_set_promise_substitute_callback(dyntracer,
                                              dyntrace_promise_substitute);

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

void dyntrace_basic_call_entry(dyntracer_t* dyntracer,
                               SEXP r_call,
                               SEXP r_op,
                               SEXP r_args,
                               SEXP r_rho,
                               dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_package_t package = instrumentr_state_get_base_package(state);

    int index = instrumentr_funtab_get_index(r_op);

    instrumentr_function_t function =
        instrumentr_package_get_basic_function_by_position(package, index);

    instrumentr_call_t call =
        instrumentr_call_create(state,
                                function,
                                r_call,
                                r_rho,
                                /* TODO: fix frame position */ 0);

    if (instrumentr_function_is_builtin(function)) {
        instrumentr_trace_builtin_call_entry(
            tracer, application, package, function, call);
    } else {
        instrumentr_trace_special_call_entry(
            tracer, application, package, function, call);
    }

    /* NOTE: release call here because it is now owned by the stack frame */
    instrumentr_model_release(call);
}

void dyntrace_basic_call_exit(dyntracer_t* dyntracer,
                              SEXP r_call,
                              SEXP r_op,
                              SEXP r_args,
                              SEXP r_rho,
                              dyntrace_dispatch_t dispatch,
                              SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    if (!instrumentr_frame_is_call(frame)) {
        instrumentr_log_error("expected call on stack");
    }

    instrumentr_call_t call = instrumentr_frame_as_call(frame);
    instrumentr_call_set_result(call, r_result);

    instrumentr_function_t function = instrumentr_call_get_function(call);

    /* TODO: add op check here as well */
    if (instrumentr_call_get_environment(call) != r_rho ||
        instrumentr_call_get_expression(call) != r_call) {
        instrumentr_log_error(
            "call on stack does not match the call being exited");
    }

    instrumentr_package_t package = instrumentr_state_get_base_package(state);

    /* TODO attach result to call */
    if (!strcmp(instrumentr_function_get_name(function), "function") &&
        TYPEOF(r_result) == CLOSXP) {
        instrumentr_state_function_table_add(state, r_result);
    }

    if (instrumentr_function_is_builtin(function)) {
        instrumentr_trace_builtin_call_exit(
            tracer, application, package, function, call);
    } else {
        instrumentr_trace_special_call_exit(
            tracer, application, package, function, call);
    }
}

void dyntrace_closure_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    /* fix package */
    instrumentr_package_t package = instrumentr_state_get_base_package(state);

    instrumentr_function_t function =
        instrumentr_state_function_table_lookup(state, r_op, r_call);

    instrumentr_call_t call =
        instrumentr_call_create(state,
                                function,
                                r_call,
                                r_rho,
                                /* TODO: fix frame position */ 0);

    instrumentr_trace_closure_call_entry(
        tracer, application, package, function, call);

    /* NOTE: release call here because it is now owned by the stack frame */
    instrumentr_model_release(call);
}

void dyntrace_closure_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    if (!instrumentr_frame_is_call(frame)) {
        instrumentr_log_error("expected call on stack");
    }

    instrumentr_call_t call = instrumentr_frame_as_call(frame);

    instrumentr_call_set_result(call, r_result);

    instrumentr_function_t function = instrumentr_call_get_function(call);

    /* TODO: add op check here as well */
    if (instrumentr_call_get_environment(call) != r_rho ||
        instrumentr_call_get_expression(call) != r_call ||
        instrumentr_function_get_definition(function).sexp != r_op) {
        instrumentr_log_error(
            "call on stack does not match the call being exited");
    }

    /* TODO: fix package  */
    instrumentr_package_t package = instrumentr_state_get_base_package(state);

    instrumentr_trace_closure_call_exit(
        tracer, application, package, function, call);
}

void dyntrace_context_entry(dyntracer_t* dyntracer, void* pointer) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_context_t context = instrumentr_context_create(state, pointer);

    instrumentr_frame_t frame =
        instrumentr_frame_create_from_context(state, context);
    instrumentr_model_release(context);

    // fprintf(stderr, "++ %p\n", pointer);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_push_frame(call_stack, frame);
    instrumentr_model_release(frame);

    instrumentr_trace_context_entry(tracer, application, context);
}

void dyntrace_context_exit(dyntracer_t* dyntracer, void* pointer) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    // fprintf(stderr, "-- %p\n", pointer);
    if (instrumentr_frame_is_context(frame)) {
        instrumentr_context_t context = instrumentr_frame_as_context(frame);

        void* actual_pointer = instrumentr_context_get_pointer(context);
        if (actual_pointer == pointer) {
            instrumentr_trace_context_exit(tracer, application, context);
            instrumentr_call_stack_pop_frame(call_stack);
        } else {
            instrumentr_log_error(
                "expected context pointer '%p' on call stack, received '%p'",
                pointer,
                actual_pointer);
        }
    } else {
        instrumentr_log_error("expected context frame on stack");
    }
}

void dyntrace_context_jump(dyntracer_t* dyntracer,
                           void* pointer,
                           SEXP return_value,
                           int restart) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    int frame_count =
        instrumentr_call_stack_get_jumped_frame_count(call_stack, pointer);

    // TODO
    // instrumentr_trace_context_jump(
    //     tracer, application, frame_count, return_value);

    while (frame_count != 0) {
        instrumentr_frame_t frame =
            instrumentr_call_stack_peek_frame(call_stack, 0);
        /* context */
        if (instrumentr_frame_is_context(frame)) {
            instrumentr_context_t context = instrumentr_frame_as_context(frame);
            instrumentr_call_stack_pop_frame(call_stack);
            // instrumentr_trace_context_exit(tracer, application, context);
        }
        /* call */
        else if (instrumentr_frame_is_call(frame)) {
            /*TODO: set call as interrupted */
            instrumentr_call_t call = instrumentr_frame_as_call(frame);
            instrumentr_function_t function =
                instrumentr_call_get_function(call);
            instrumentr_package_t package =
                instrumentr_state_get_base_package(state);

            /* builtin */
            if (instrumentr_function_is_builtin(function)) {
                instrumentr_trace_builtin_call_exit(
                    tracer, application, package, function, call);
            }
            /* special */
            else if (instrumentr_function_is_special(function)) {
                instrumentr_trace_special_call_exit(
                    tracer, application, package, function, call);
            }
            /* closure */
            else if (instrumentr_function_is_closure(function)) {
                instrumentr_trace_closure_call_exit(
                    tracer, application, package, function, call);
            }
        }
        /* promise */
        else if (instrumentr_frame_is_promise(frame)) {
            instrumentr_promise_t promise = instrumentr_frame_as_promise(frame);
            instrumentr_trace_promise_force_exit(tracer, promise);
        }
        --frame_count;
    }
}

void dyntrace_gc_deallocate(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    if (TYPEOF(r_object) == CLOSXP) {
        instrumentr_application_t application =
            instrumentr_tracer_get_application(tracer);
        instrumentr_state_function_table_remove(state, r_object);
    }
    else if (TYPEOF(r_object) == PROMSXP) {
        instrumentr_state_promise_table_remove(state, r_object);
    }
}

void dyntrace_promise_force_entry(dyntracer_t* dyntracer, SEXP r_promise) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    instrumentr_trace_promise_force_entry(tracer, promise);
}

void dyntrace_promise_force_exit(dyntracer_t* dyntracer, SEXP r_promise) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    instrumentr_trace_promise_force_exit(tracer, promise);
}

void dyntrace_promise_value_lookup(dyntracer_t* dyntracer, SEXP r_promise) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    instrumentr_trace_promise_value_lookup(tracer, promise);
}

void dyntrace_promise_substitute(dyntracer_t* dyntracer, SEXP r_promise) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    instrumentr_trace_promise_substitute(tracer, promise);
}
