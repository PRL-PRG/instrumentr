#include <instrumentr/types.h>
#include "instrumentr_internals.h"

#include "object.h"
#include "tracer.h"
#include "callback.h"
#include "application.h"
#include "function.h"
#include "environment.h"
#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "trace.h"
#include "dyntrace.h"
#include "utilities.h"
#include "interop.h"
#include "funtab.h"
#include "context.h"
#include "frame.h"
#include "state.h"
#include "call_stack.h"
#include "promise.h"
#include "exec_stats.h"
#include "event.h"
#include "miscellaneous.h"

#define TRACING_INITIALIZE(EVENT)                                     \
    instrumentr_tracer_disable(tracer);                               \
    clock_t begin = clock();                                          \
    instrumentr_state_t state = instrumentr_tracer_get_state(tracer); \
    instrumentr_state_increment_time(state);                          \
    instrumentr_application_t application =                           \
        instrumentr_tracer_get_application(tracer);

#define TRACING_FINALIZE(EVENT)                             \
    instrumentr_exec_stats_t exec_stats =                   \
        instrumentr_state_get_exec_stats(state);            \
    clock_t end = clock();                                  \
    clock_t diff = end - begin;                             \
    instrumentr_exec_stats_update(exec_stats, EVENT, diff); \
    instrumentr_tracer_reinstate(tracer);

// definition for FOO
#define TRACING_INVOKE_R_CALLBACK(...) \
    OVERLOADED_MACRO(TRACING_INVOKE_R_CALLBACK, __VA_ARGS__)

#define TRACING_INVOKE_R_CALLBACK_0(ONE)                                    \
    Rf_eval(Rf_lang5(r_name, r_tracer, r_callback, r_state, r_application), \
            r_trace_env);

#define TRACING_INVOKE_R_CALLBACK_1(ONE)                                    \
    SEXP r_##ONE = PROTECT(instrumentr_##ONE##_wrap(ONE));                  \
    Rf_eval(                                                                \
        Rf_lang6(                                                           \
            r_name, r_tracer, r_callback, r_state, r_application, r_##ONE), \
        r_trace_env);                                                       \
    UNPROTECT(1);

#define TRACING_INVOKE_R_CALLBACK_2(ONE, TWO)              \
    SEXP r_##ONE = PROTECT(instrumentr_##ONE##_wrap(ONE)); \
    SEXP r_##TWO = PROTECT(instrumentr_##TWO##_wrap(TWO)); \
    Rf_eval(Rf_lang7(r_name,                               \
                     r_tracer,                             \
                     r_callback,                           \
                     r_state,                              \
                     r_application,                        \
                     r_##ONE,                              \
                     r_##TWO),                             \
            r_trace_env);                                  \
    UNPROTECT(2);

#define TRACING_INVOKE_R_CALLBACK_3(ONE, TWO, THREE)             \
    SEXP r_##ONE = PROTECT(instrumentr_##ONE##_wrap(ONE));       \
    SEXP r_##TWO = PROTECT(instrumentr_##TWO##_wrap(TWO));       \
    SEXP r_##THREE = PROTECT(instrumentr_##THREE##_wrap(THREE)); \
    Rf_eval(Rf_lang3(r_name,                                     \
                     r_tracer,                                   \
                     r_callback,                                 \
                     r_state,                                    \
                     r_application,                              \
                     r_##ONE,                                    \
                     r_##TWO,                                    \
                     r_##THREE),                                 \
            r_trace_env);                                        \
    UNPROTECT(3);

#define TRACING_INVOKE_CALLBACK(EVENT, TYPE, ...)                             \
    if (instrumentr_tracer_has_callback(tracer, EVENT)) {                     \
        instrumentr_callback_t callback =                                     \
            instrumentr_tracer_get_callback(tracer, EVENT);                   \
                                                                              \
        instrumentr_tracer_set_active_callback(tracer, callback);             \
        instrumentr_callback_activate(callback);                              \
                                                                              \
        if (instrumentr_callback_has_c_function(callback)) {                  \
            TYPE cfun = (TYPE) instrumentr_callback_get_c_function(callback); \
            cfun(tracer, callback, state, application, ##__VA_ARGS__);        \
        } else {                                                              \
            SEXP r_tracer = PROTECT(instrumentr_tracer_wrap(tracer));         \
            SEXP r_application =                                              \
                PROTECT(instrumentr_application_wrap(application));           \
            SEXP r_state = PROTECT(instrumentr_state_wrap(state));            \
            SEXP r_callback = PROTECT(instrumentr_callback_wrap(callback));   \
            const char* name = instrumentr_event_to_string(EVENT);            \
            SEXP r_name = PROTECT(Rf_install(name));                          \
            SEXP r_trace_env = instrumentr_tracer_get_environment(tracer);    \
            TRACING_INVOKE_R_CALLBACK(__VA_ARGS__)                            \
            UNPROTECT(5);                                                     \
        }                                                                     \
                                                                              \
        instrumentr_callback_deactivate(callback);                            \
        instrumentr_tracer_unset_active_callback(tracer);                     \
    }

SEXP r_instrumentr_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    clock_t start = clock();

    SEXP r_result = R_NilValue;

    instrumentr_tracer_enable(tracer);

    r_result = dyntrace_trace_code(instrumentr_tracer_get_dyntracer(tracer),
                                   r_code,
                                   r_environment)
                   .value;

    instrumentr_tracer_reinstate(tracer);

    clock_t end = clock();

    clock_t diff = end - start;

    instrumentr_exec_stats_t exec_stats =
        instrumentr_state_get_exec_stats(state);
    instrumentr_exec_stats_update(exec_stats, INSTRUMENTR_EVENT_COUNT, diff);

    return r_result;
}

SEXP r_instrumentr_trace_tracing_entry(SEXP r_tracer,
                                       SEXP r_workdir,
                                       SEXP r_code,
                                       SEXP r_appenv) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_TRACING_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    /* NOTE: before TRACING_INITIALIZE because it creates application */
    instrumentr_tracer_initialize_tracing(
        tracer, CHAR(STRING_ELT(r_workdir, 0)), r_code, r_appenv);

    TRACING_INITIALIZE(event)

    /* TODO add program's return value here or error message? */
    TRACING_INVOKE_CALLBACK(event, tracing_entry_function_t);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_tracing_exit(SEXP r_tracer) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_TRACING_EXIT;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    /* TODO add program's return value here or error message? */
    TRACING_INVOKE_CALLBACK(event, tracing_exit_function_t);

    TRACING_FINALIZE(event)

    /* NOTE: finalize before getting exec_stats because finalization updates
     * exec_stats */
    SEXP r_result = PROTECT(instrumentr_tracer_finalize_tracing(tracer));

    UNPROTECT(1);

    return r_result;
}

SEXP r_instrumentr_trace_package_load(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_LOAD;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_update_namespace(state,
                                                             package_name);

    TRACING_INVOKE_CALLBACK(event, package_load_function_t, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_unload(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_UNLOAD;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_lookup_namespace(state,
                                                             package_name);

    TRACING_INVOKE_CALLBACK(event, package_unload_function_t, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_attach(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_ATTACH;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_lookup_package(state, package_name);

    TRACING_INVOKE_CALLBACK(event, package_attach_function_t, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_detach(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_DETACH;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_environment_table_lookup_package(state, package_name);

    TRACING_INVOKE_CALLBACK(event, package_detach_function_t, environment)

    TRACING_FINALIZE(event)

    return R_NilValue;
}

void instrumentr_trace_call_entry(dyntracer_t* dyntracer,
                                  SEXP r_call,
                                  SEXP r_op,
                                  SEXP r_args,
                                  SEXP r_rho,
                                  dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event;

    if (TYPEOF(r_op) == CLOSXP) {
        event = INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY;
    } else if (TYPEOF(r_op) == BUILTINSXP) {
        event = INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY;
    } else {
        event = INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY;
    }

    TRACING_INITIALIZE(event)

    instrumentr_function_t function =
        instrumentr_state_function_table_lookup(state, r_op, 1);

    instrumentr_environment_t environment =
        instrumentr_function_get_environment(function);

    instrumentr_call_t call =
        instrumentr_call_create(state,
                                function,
                                r_call,
                                r_rho,
                                /* TODO: fix frame position */ 0);

    instrumentr_frame_t frame = instrumentr_frame_create_from_call(state, call);
    /* NOTE: release call here because it is now owned by the stack frame */
    instrumentr_model_release(call);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);
    instrumentr_call_stack_push_frame(call_stack, frame);
    instrumentr_model_release(frame);

    if (event == INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY) {
        TRACING_INVOKE_CALLBACK(
            event, closure_call_entry_function_t, function, call);
    } else if (event == INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY) {
        TRACING_INVOKE_CALLBACK(
            event, builtin_call_entry_function_t, function, call);
    } else {
        TRACING_INVOKE_CALLBACK(
            event, special_call_entry_function_t, function, call);
    }

    TRACING_FINALIZE(event)
}

void instrumentr_trace_builtin_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
                            builtin_call_exit_function_t,
                            function,
                            call);
}

void instrumentr_trace_special_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
                            special_call_exit_function_t,
                            function,
                            call);
}

void instrumentr_trace_closure_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_state_t state,
                                         instrumentr_application_t application,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
                            closure_call_exit_function_t,
                            function,
                            call);
}

void instrumentr_trace_call_exit(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch,
                                 SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event;

    if (TYPEOF(r_op) == BUILTINSXP) {
        event = INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT;
    } else if (TYPEOF(r_op) == SPECIALSXP) {
        event = INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT;
    } else {
        event = INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT;
    }

    TRACING_INITIALIZE(event)

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

    instrumentr_environment_t environment =
        instrumentr_function_get_environment(function);

    instrumentr_environment_t call_env = instrumentr_call_get_environment(call);

    /* TODO: add op check here as well */
    if (instrumentr_environment_get_environment(call_env) != r_rho ||
        instrumentr_call_get_expression(call) != r_call) {
        instrumentr_log_error(
            "call on stack does not match the call being exited");
    }

    if (instrumentr_function_get_name(function) != NULL &&
        !strcmp(instrumentr_function_get_name(function), "function") &&
        TYPEOF(r_result) == CLOSXP) {
        instrumentr_state_function_table_create(state, r_result);
    }

    if (event == INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT) {
        instrumentr_trace_builtin_call_exit(
            tracer, state, application, function, call);

    } else if (event == INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT) {
        instrumentr_trace_special_call_exit(
            tracer, state, application, function, call);

    } else {
        instrumentr_trace_closure_call_exit(
            tracer, state, application, function, call);
    }

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_context_entry(dyntracer_t* dyntracer, void* pointer) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_CONTEXT_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_context_t context = instrumentr_context_create(state, pointer);

    instrumentr_frame_t frame =
        instrumentr_frame_create_from_context(state, context);
    instrumentr_model_release(context);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_push_frame(call_stack, frame);
    instrumentr_model_release(frame);

    TRACING_INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_CONTEXT_ENTRY, context_entry_function_t, context);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_context_exit(dyntracer_t* dyntracer, void* pointer) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_CONTEXT_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    if (!instrumentr_frame_is_context(frame)) {
        instrumentr_log_error("expected context frame on stack");
    }

    instrumentr_context_t context = instrumentr_frame_as_context(frame);

    void* actual_pointer = instrumentr_context_get_pointer(context);

    if (actual_pointer != pointer) {
        instrumentr_log_error(
            "expected context pointer '%p' on call stack, received '%p'",
            pointer,
            actual_pointer);
    }

    TRACING_INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_CONTEXT_EXIT, context_exit_function_t, context);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_context_jump(dyntracer_t* dyntracer,
                                    void* pointer,
                                    SEXP return_value,
                                    int restart) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_CONTEXT_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    int frame_count =
        instrumentr_call_stack_get_jumped_frame_count(call_stack, pointer);

    // TODO: add after modeling value
    // TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CONTEXT_JUMP,
    //                        context_jump_function_t,
    //                        context,
    //                        return_value);

    // TODO
    // instrumentr_trace_context_jump(
    //     tracer, application, frame_count, return_value);

    // TODO: add return value to last frame
    while (frame_count != 0) {
        instrumentr_frame_t frame =
            instrumentr_call_stack_peek_frame(call_stack, 0);
        /* context */
        if (instrumentr_frame_is_context(frame)) {
            instrumentr_context_t context = instrumentr_frame_as_context(frame);

            TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CONTEXT_EXIT,
                                    context_exit_function_t,
                                    context);

        }
        /* call */
        else if (instrumentr_frame_is_call(frame)) {
            /*TODO: set call as interrupted */
            instrumentr_call_t call = instrumentr_frame_as_call(frame);

            instrumentr_function_t function =
                instrumentr_call_get_function(call);

            /* builtin */
            if (instrumentr_function_is_builtin(function)) {
                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
                                        builtin_call_exit_function_t,
                                        function,
                                        call);

            }
            /* special */
            else if (instrumentr_function_is_special(function)) {
                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
                                        special_call_exit_function_t,
                                        function,
                                        call);

            }
            /* closure */
            else if (instrumentr_function_is_closure(function)) {
                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
                                        closure_call_exit_function_t,
                                        function,
                                        call);
            }
        }
        /* promise */
        else if (instrumentr_frame_is_promise(frame)) {
            instrumentr_promise_t promise = instrumentr_frame_as_promise(frame);

            TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT,
                                    promise_force_exit_function_t,
                                    promise);
        }

        instrumentr_call_stack_pop_frame(call_stack);
        --frame_count;
    }

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_force_entry(dyntracer_t* dyntracer,
                                           SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_FORCE_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_frame_create_from_promise(state, promise);

    instrumentr_call_stack_push_frame(call_stack, frame);

    instrumentr_model_release(frame);

    TRACING_INVOKE_CALLBACK(event, promise_force_entry_function_t, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_force_exit(dyntracer_t* dyntracer,
                                          SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(event, promise_force_exit_function_t, promise);

    /* TODO: check that stack has promise. */
    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_value_lookup(dyntracer_t* dyntracer,
                                            SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_VALUE_LOOKUP;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(event, promise_value_lookup_function_t, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_substitute(dyntracer_t* dyntracer,
                                          SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_SUBSTITUTE;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(event, promise_substitute_function_t, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_eval_entry(dyntracer_t* dyntracer,
                                  SEXP r_expression,
                                  SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_EVAL_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add after adding value
    // TRACING_INVOKE_CALLBACK(event, eval_entry_function_t, expression, rho);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_eval_exit(dyntracer_t* dyntracer,
                                 SEXP r_expression,
                                 SEXP r_rho,
                                 SEXP r_result) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_EVAL_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add after adding value
    // TRACING_INVOKE_CALLBACK(event, eval_entry_function_t, expression, rho,
    // result);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_definition(dyntracer_t* dyntracer,
                                           const SEXP r_symbol,
                                           const SEXP r_value,
                                           const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_DEFINITION;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add back this line:
    // instrumentr_state_function_table_update_name(state, r_symbol,
    // r_value, r_rho);),

    // TODO: add back this line.
    // TRACING_INVOKE_CALLBACK(event, variable_definition_function_t, symbol,
    // value, rho);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_assignment(dyntracer_t* dyntracer,
                                           const SEXP r_symbol,
                                           const SEXP r_value,
                                           const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add back this line:
    // instrumentr_state_function_table_update_name(state, r_symbol,
    // r_value, r_rho);),

    // TODO: add back this line.
    // TRACING_INVOKE_CALLBACK(event, variable_definition_function_t, symbol,
    // value, rho);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_removal(dyntracer_t* dyntracer,
                                        const SEXP r_symbol,
                                        const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_REMOVAL;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add back this line.
    // TRACING_INVOKE_CALLBACK(event, variable_removal_function_t, symbol,
    // value, rho);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_lookup(dyntracer_t* dyntracer,
                                       const SEXP r_symbol,
                                       const SEXP r_value,
                                       const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_LOOKUP;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    // TODO: add back this line.
    // TRACING_INVOKE_CALLBACK(event, variable_lookup_function_t, symbol,
    // value, rho);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_GC_ALLOCATION;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    if (TYPEOF(r_object) == PROMSXP) {
        instrumentr_state_promise_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CLOSXP) {
        instrumentr_state_function_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == SPECIALSXP) {
        instrumentr_state_function_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == BUILTINSXP) {
        instrumentr_state_function_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == ENVSXP) {
        instrumentr_state_environment_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == INTSXP) {
        instrumentr_state_integer_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == REALSXP) {
        instrumentr_state_real_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == LGLSXP) {
        instrumentr_state_logical_table_create(state, r_object);
    }

    else if (TYPEOF(r_object) == CPLXSXP) {
        instrumentr_state_complex_table_create(state, r_object);
    }

    else {
        instrumentr_state_miscellaneous_table_create(state, r_object);
    }

    /* TODO: add this after adding value */
    // TRACING_INVOKE_CALLBACK(event, gc_allocation_function_t, value);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_gc_deallocation(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_GC_DEALLOCATION;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    if (TYPEOF(r_object) == PROMSXP) {
        instrumentr_state_promise_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == CLOSXP) {
        instrumentr_state_function_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == SPECIALSXP) {
        instrumentr_state_function_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == BUILTINSXP) {
        instrumentr_state_function_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == ENVSXP) {
        instrumentr_state_environment_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == INTSXP) {
        instrumentr_state_integer_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == REALSXP) {
        instrumentr_state_real_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == LGLSXP) {
        instrumentr_state_logical_table_remove(state, r_object);
    }

    else if (TYPEOF(r_object) == CPLXSXP) {
        instrumentr_state_complex_table_remove(state, r_object);
    }

    else {
        instrumentr_state_miscellaneous_table_remove(state, r_object);
    }

    /* TODO: add this after adding value */
    // TRACING_INVOKE_CALLBACK(event, gc_deallocation_function_t, value);

    TRACING_FINALIZE(event)
}
