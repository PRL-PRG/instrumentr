#include <instrumentr/types.h>
#include "instrumentr_internals.h"

#include "object.h"
#include "tracer.h"
#include "callback.h"
#include "application.h"
#include "environment.h"
#include "call.h"
#include "trace.h"
#include "dyntrace.h"
#include "utilities.h"
#include "interop.h"
#include "funtab.h"
#include "context.h"
#include "frame.h"
#include "state.h"
#include "call_stack.h"
#include "exec_stats.h"
#include "event.h"
#include "miscellaneous.h"
#include "value.h"
#include "values.h"
#include "eval.h"

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

#define TRACING_INVOKE_R_CALLBACK(...) \
    OVERLOADED_MACRO(TRACING_INVOKE_R_CALLBACK, __VA_ARGS__)

#define TRACING_INVOKE_R_CALLBACK_0(ONE)                                    \
    Rf_eval(Rf_lang5(r_name, r_tracer, r_callback, r_state, r_application), \
            r_trace_env);

#define TRACING_INVOKE_R_CALLBACK_2(ONE_NAME, ONE_TYPE)                   \
    SEXP r_##ONE_NAME = PROTECT(instrumentr_##ONE_TYPE##_wrap(ONE_NAME)); \
    Rf_eval(Rf_lang6(r_name,                                              \
                     r_tracer,                                            \
                     r_callback,                                          \
                     r_state,                                             \
                     r_application,                                       \
                     r_##ONE_NAME),                                       \
            r_trace_env);                                                 \
    UNPROTECT(1);

#define TRACING_INVOKE_R_CALLBACK_4(ONE_NAME, ONE_TYPE, TWO_NAME, TWO_TYPE) \
    SEXP r_##ONE_NAME = PROTECT(instrumentr_##ONE_TYPE##_wrap(ONE_NAME));   \
    SEXP r_##TWO_NAME = PROTECT(instrumentr_##TWO_TYPE##_wrap(TWO_NAME));   \
    Rf_eval(Rf_lang7(r_name,                                                \
                     r_tracer,                                              \
                     r_callback,                                            \
                     r_state,                                               \
                     r_application,                                         \
                     r_##ONE_NAME,                                          \
                     r_##TWO_NAME),                                         \
            r_trace_env);                                                   \
    UNPROTECT(2);

#define TRACING_INVOKE_R_CALLBACK_6(                                      \
    ONE_NAME, ONE_TYPE, TWO_NAME, TWO_TYPE, THREE_NAME, THREE_TYPE)       \
    SEXP r_##ONE_NAME = PROTECT(instrumentr_##ONE_TYPE##_wrap(ONE_NAME)); \
    SEXP r_##TWO_NAME = PROTECT(instrumentr_##TWO_TYPE##_wrap(TWO_NAME)); \
    SEXP r_##THREE_NAME =                                                 \
        PROTECT(instrumentr_##THREE_TYPE##_wrap(THREE_NAME));             \
    Rf_eval(Rf_lang8(r_name,                                              \
                     r_tracer,                                            \
                     r_callback,                                          \
                     r_state,                                             \
                     r_application,                                       \
                     r_##ONE_NAME,                                        \
                     r_##TWO_NAME,                                        \
                     r_##THREE_NAME),                                     \
            r_trace_env);                                                 \
    UNPROTECT(3);

#define TRACING_INVOKE_C_CALLBACK(...) \
    OVERLOADED_MACRO(TRACING_INVOKE_C_CALLBACK, __VA_ARGS__)

#define TRACING_INVOKE_C_CALLBACK_0(ONE) \
    cfun(tracer, callback, state, application);

#define TRACING_INVOKE_C_CALLBACK_2(ONE_NAME, ONE_TYPE) \
    cfun(tracer, callback, state, application, ONE_NAME);

#define TRACING_INVOKE_C_CALLBACK_4(ONE_NAME, ONE_TYPE, TWO_NAME, TWO_TYPE) \
    cfun(tracer, callback, state, application, ONE_NAME, TWO_NAME);

#define TRACING_INVOKE_C_CALLBACK_6(                                \
    ONE_NAME, ONE_TYPE, TWO_NAME, TWO_TYPE, THREE_NAME, THREE_TYPE) \
    cfun(tracer, callback, state, application, ONE_NAME, TWO_NAME, THREE_NAME);

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
            TRACING_INVOKE_C_CALLBACK(__VA_ARGS__)                            \
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

    dyntrace_result_t result;

    int prot = 0;
    SEXP r_value = R_NilValue;
    SEXP r_error = PROTECT(ScalarLogical(TRUE));
    ++prot;

    instrumentr_tracer_enable(tracer);

    result = dyntrace_trace_code(
        instrumentr_tracer_get_dyntracer(tracer), r_code, r_environment);

    instrumentr_tracer_reinstate(tracer);

    /* this means error has not occurred */
    if (!result.error_code) {
        r_value = PROTECT(result.value);
        ++prot;
        r_error = PROTECT(ScalarLogical(FALSE));
        ++prot;
    }

    clock_t end = clock();

    clock_t diff = end - start;

    instrumentr_exec_stats_t exec_stats =
        instrumentr_state_get_exec_stats(state);
    instrumentr_exec_stats_update(exec_stats, INSTRUMENTR_EVENT_COUNT, diff);

    SEXP r_result = PROTECT(allocVector(VECSXP, 2));
    ++prot;
    SET_VECTOR_ELT(r_result, 0, r_value);
    SET_VECTOR_ELT(r_result, 1, r_error);

    UNPROTECT(prot);

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
    instrumentr_trace_multivalue_finalize(r_tracer);

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
        instrumentr_state_value_table_update_namespace(state, package_name);

    TRACING_INVOKE_CALLBACK(
        event, package_load_function_t, environment, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_unload(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_UNLOAD;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_namespace(state, package_name);

    TRACING_INVOKE_CALLBACK(
        event, package_unload_function_t, environment, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_attach(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_ATTACH;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_package(state, package_name);

    TRACING_INVOKE_CALLBACK(
        event, package_attach_function_t, environment, environment);

    TRACING_FINALIZE(event)

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_detach(SEXP r_tracer, SEXP r_package_name) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PACKAGE_DETACH;

    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    TRACING_INITIALIZE(event)

    const char* package_name = CHAR(STRING_ELT(r_package_name, 0));

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_package(state, package_name);

    TRACING_INVOKE_CALLBACK(
        event, package_detach_function_t, environment, environment)

    TRACING_FINALIZE(event)

    return R_NilValue;
}

instrumentr_call_t instrumentr_trace_call_entry(instrumentr_state_t state,
                                                instrumentr_value_t function,
                                                SEXP r_call,
                                                SEXP r_args,
                                                SEXP r_rho) {
    instrumentr_call_t call =
        instrumentr_call_create(state, function, r_call, r_args, r_rho);

    instrumentr_frame_t frame = instrumentr_frame_create_from_call(state, call);
    /* NOTE: release call here because it is now owned by the stack frame */
    instrumentr_call_release(call);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_push_frame(call_stack, frame);

    instrumentr_frame_release(frame);

    return call;
}

void instrumentr_trace_builtin_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY;

    TRACING_INITIALIZE(event)

    instrumentr_value_t function =
        instrumentr_state_value_table_lookup(state, r_op, 1);

    instrumentr_builtin_t builtin = instrumentr_value_as_builtin(function);

    instrumentr_call_t call =
        instrumentr_trace_call_entry(state, function, r_call, r_args, r_rho);

    TRACING_INVOKE_CALLBACK(
        event, builtin_call_entry_function_t, builtin, builtin, call, call);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_special_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY;

    TRACING_INITIALIZE(event)

    instrumentr_value_t function =
        instrumentr_state_value_table_lookup(state, r_op, 1);

    instrumentr_special_t special = instrumentr_value_as_special(function);

    instrumentr_call_t call =
        instrumentr_trace_call_entry(state, function, r_call, r_args, r_rho);

    TRACING_INVOKE_CALLBACK(
        event, special_call_entry_function_t, special, special, call, call);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_closure_call_entry(dyntracer_t* dyntracer,
                                          SEXP r_call,
                                          SEXP r_op,
                                          SEXP r_args,
                                          SEXP r_rho,
                                          dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY;

    TRACING_INITIALIZE(event)

    instrumentr_value_t function =
        instrumentr_state_value_table_lookup(state, r_op, 1);

    instrumentr_closure_t closure = instrumentr_value_as_closure(function);

    /* get this to make sure closure environment is constructed */
    instrumentr_environment_t cloenv =
        instrumentr_closure_get_environment(closure);

    instrumentr_call_t call =
        instrumentr_trace_call_entry(state, function, r_call, r_args, r_rho);

    instrumentr_environment_t callenv = instrumentr_call_get_environment(call);

    instrumentr_environment_set_call(callenv, call);

    TRACING_INVOKE_CALLBACK(
        event, closure_call_entry_function_t, closure, closure, call, call);

    TRACING_FINALIZE(event)
}

instrumentr_call_t instrumentr_trace_call_exit(instrumentr_state_t state,
                                               SEXP r_call,
                                               SEXP r_rho,
                                               SEXP r_result) {
    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    if (!instrumentr_frame_is_call(frame)) {
        instrumentr_log_error("expected call on stack");
    }

    instrumentr_call_t call = instrumentr_frame_as_call(frame);

    instrumentr_environment_t environment =
        instrumentr_call_get_environment(call);

    if (instrumentr_language_get_sexp(instrumentr_call_get_expression(call)) !=
            r_call ||
        instrumentr_environment_get_sexp(environment) != r_rho) {
        instrumentr_log_error(
            "call on stack does not match the call being exited");
    }

    instrumentr_value_t result =
        instrumentr_state_value_table_lookup(state, r_result, 1);
    instrumentr_call_set_result(call, result);

    return call;
}

void instrumentr_trace_builtin_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT;

    TRACING_INITIALIZE(event)

    instrumentr_call_t call =
        instrumentr_trace_call_exit(state, r_call, r_rho, r_result);

    instrumentr_value_t function = instrumentr_call_get_function(call);

    instrumentr_builtin_t builtin = instrumentr_value_as_builtin(function);

    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
                            builtin_call_exit_function_t,
                            builtin,
                            builtin,
                            call,
                            call);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_special_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT;

    TRACING_INITIALIZE(event)

    instrumentr_call_t call =
        instrumentr_trace_call_exit(state, r_call, r_rho, r_result);

    instrumentr_value_t function = instrumentr_call_get_function(call);

    instrumentr_special_t special = instrumentr_value_as_special(function);

    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
                            special_call_exit_function_t,
                            special,
                            special,
                            call,
                            call);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_closure_call_exit(dyntracer_t* dyntracer,
                                         SEXP r_call,
                                         SEXP r_op,
                                         SEXP r_args,
                                         SEXP r_rho,
                                         dyntrace_dispatch_t dispatch,
                                         SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT;

    TRACING_INITIALIZE(event)

    instrumentr_call_t call =
        instrumentr_trace_call_exit(state, r_call, r_rho, r_result);

    instrumentr_value_t function = instrumentr_call_get_function(call);

    instrumentr_closure_t closure = instrumentr_value_as_closure(function);

    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
                            closure_call_exit_function_t,
                            closure,
                            closure,
                            call,
                            call);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_use_method_entry(dyntracer_t* dyntracer,
                                        const char* generic,
                                        SEXP r_klass,
                                        SEXP r_object,
                                        SEXP r_call,
                                        SEXP r_args,
                                        SEXP r_rho) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_USE_METHOD_ENTRY;

    TRACING_INITIALIZE(event)

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_object, 1);

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_environment(state, r_rho, 1);

    TRACING_INVOKE_CALLBACK(event,
                            use_method_entry_function_t,
                            value,
                            value,
                            environment,
                            environment);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_use_method_exit(dyntracer_t* dyntracer,
                                       const char* generic,
                                       SEXP r_klass,
                                       SEXP r_object,
                                       SEXP r_call,
                                       SEXP r_args,
                                       SEXP r_rho,
                                       SEXP r_ans) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_USE_METHOD_ENTRY;

    TRACING_INITIALIZE(event)

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_object, 1);

    instrumentr_environment_t environment =
        instrumentr_state_value_table_lookup_environment(state, r_rho, 1);

    // TODO: add optional_value
    instrumentr_value_t ans =
        r_ans == NULL ? NULL
                      : instrumentr_state_value_table_lookup(state, r_ans, 1);

    TRACING_INVOKE_CALLBACK(event,
                            use_method_exit_function_t,
                            value,
                            value,
                            environment,
                            environment,
                            ans,
                            value);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_context_entry(dyntracer_t* dyntracer, void* pointer) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_CONTEXT_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_context_t context = instrumentr_context_create(state, pointer);

    instrumentr_frame_t frame =
        instrumentr_frame_create_from_context(state, context);
    instrumentr_context_release(context);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_push_frame(call_stack, frame);
    instrumentr_frame_release(frame);

    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CONTEXT_ENTRY,
                            context_entry_function_t,
                            context,
                            context);

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

    TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CONTEXT_EXIT,
                            context_exit_function_t,
                            context,
                            context);

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
                                    context,
                                    context);

        }
        /* call */
        else if (instrumentr_frame_is_call(frame)) {
            /*TODO: set call as interrupted */
            instrumentr_call_t call = instrumentr_frame_as_call(frame);

            instrumentr_value_t function = instrumentr_call_get_function(call);

            /* builtin */
            if (instrumentr_value_is_builtin(function)) {
                instrumentr_builtin_t builtin =
                    instrumentr_value_as_builtin(function);

                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
                                        builtin_call_exit_function_t,
                                        builtin,
                                        builtin,
                                        call,
                                        call);

            }
            /* special */
            else if (instrumentr_value_is_special(function)) {
                instrumentr_special_t special =
                    instrumentr_value_as_special(function);

                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
                                        special_call_exit_function_t,
                                        special,
                                        special,
                                        call,
                                        call);

            }
            /* closure */
            else if (instrumentr_value_is_closure(function)) {
                instrumentr_closure_t closure =
                    instrumentr_value_as_closure(function);

                TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
                                        closure_call_exit_function_t,
                                        closure,
                                        closure,
                                        call,
                                        call);
            }
        }
        /* promise */
        else if (instrumentr_frame_is_promise(frame)) {
            instrumentr_promise_t promise = instrumentr_frame_as_promise(frame);

            instrumentr_promise_set_force_exit_time(
                promise, instrumentr_state_get_time(state));

            TRACING_INVOKE_CALLBACK(INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT,
                                    promise_force_exit_function_t,
                                    promise,
                                    promise);
        }
        /* eval */
        else if (instrumentr_frame_is_eval(frame)) {
            instrumentr_eval_t eval = instrumentr_frame_as_eval(frame);

            TRACING_INVOKE_CALLBACK(
                INSTRUMENTR_EVENT_EVAL_EXIT, eval_exit_function_t, eval, eval);
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
        instrumentr_state_value_table_lookup_promise(state, r_promise, 1);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_frame_create_from_promise(state, promise);

    instrumentr_call_stack_push_frame(call_stack, frame);

    instrumentr_frame_release(frame);

    instrumentr_promise_set_force_entry_time(promise,
                                             instrumentr_state_get_time(state));

    TRACING_INVOKE_CALLBACK(
        event, promise_force_entry_function_t, promise, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_force_exit(dyntracer_t* dyntracer,
                                          SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_value_table_lookup_promise(state, r_promise, 1);

    instrumentr_promise_set_force_exit_time(promise,
                                            instrumentr_state_get_time(state));

    TRACING_INVOKE_CALLBACK(
        event, promise_force_exit_function_t, promise, promise);

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
        instrumentr_state_value_table_lookup_promise(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(
        event, promise_value_lookup_function_t, promise, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_expression_lookup(dyntracer_t* dyntracer,
                                                 SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_EXPRESSION_LOOKUP;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_value_table_lookup_promise(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(
        event, promise_expression_lookup_function_t, promise, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_substitute(dyntracer_t* dyntracer,
                                          SEXP r_promise) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_SUBSTITUTE;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_promise_t promise =
        instrumentr_state_value_table_lookup_promise(state, r_promise, 1);

    TRACING_INVOKE_CALLBACK(
        event, promise_substitute_function_t, promise, promise);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_delayed_assign(dyntracer_t* dyntracer,
                                              SEXP r_symbol,
                                              SEXP r_promise,
                                              SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_DELAYED_ASSIGN;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t promval =
        instrumentr_state_value_table_lookup(state, r_promise, 1);
    instrumentr_promise_t promise = instrumentr_value_as_promise(promval);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    TRACING_INVOKE_CALLBACK(event,
                            promise_delayed_assign_function_t,
                            symbol,
                            symbol,
                            promise,
                            promise,
                            environment,
                            environment)

    TRACING_FINALIZE(event)
}

void instrumentr_trace_promise_lazy_load(dyntracer_t* dyntracer,
                                         SEXP r_symbol,
                                         SEXP r_promise,
                                         SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_PROMISE_LAZY_LOAD;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t promval =
        instrumentr_state_value_table_lookup(state, r_promise, 1);
    instrumentr_promise_t promise = instrumentr_value_as_promise(promval);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    TRACING_INVOKE_CALLBACK(event,
                            promise_lazy_load_function_t,
                            symbol,
                            symbol,
                            promise,
                            promise,
                            environment,
                            environment)

    TRACING_FINALIZE(event)
}

void instrumentr_trace_eval_entry(dyntracer_t* dyntracer,
                                  SEXP r_expression,
                                  SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_EVAL_ENTRY;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_eval_t eval =
        instrumentr_eval_create(state, r_expression, r_rho);

    instrumentr_frame_t frame = instrumentr_frame_create_from_eval(state, eval);
    instrumentr_eval_release(eval);

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_call_stack_push_frame(call_stack, frame);
    instrumentr_frame_release(frame);

    TRACING_INVOKE_CALLBACK(event, eval_entry_function_t, eval, eval);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_eval_exit(dyntracer_t* dyntracer,
                                 SEXP r_expression,
                                 SEXP r_rho,
                                 SEXP r_result) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_EVAL_EXIT;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_call_stack_t call_stack =
        instrumentr_state_get_call_stack(state);

    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, 0);

    if (!instrumentr_frame_is_eval(frame)) {
        instrumentr_log_error("expected eval frame on stack");
    }

    instrumentr_eval_t eval = instrumentr_frame_as_eval(frame);

    SEXP r_curr_expression =
        instrumentr_value_get_sexp(instrumentr_eval_get_expression(eval));

    if (r_expression != r_curr_expression) {
        instrumentr_log_error(
            "expected expression pointer '%p' in eval frame, received '%p'",
            r_expression,
            r_curr_expression);
    }

    SEXP r_curr_rho = instrumentr_environment_get_sexp(
        instrumentr_eval_get_environment(eval));

    if (r_rho != r_curr_rho) {
        instrumentr_log_error(
            "expected environment pointer '%p' in eval frame, received '%p'",
            r_rho,
            r_curr_rho);
    }

    instrumentr_value_t result =
        instrumentr_state_value_table_lookup(state, r_result, 1);

    instrumentr_eval_set_result(eval, result);

    TRACING_INVOKE_CALLBACK(event, eval_exit_function_t, eval, eval);

    instrumentr_call_stack_pop_frame(call_stack);

    TRACING_FINALIZE(event)
}

void set_function_name(instrumentr_symbol_t symbol,
                       instrumentr_value_t value,
                       instrumentr_environment_t environment) {
    if (instrumentr_value_is_closure(value)) {
        instrumentr_closure_t closure = instrumentr_value_as_closure(value);
        if (instrumentr_closure_has_name(closure)) {
            return;
        }
        instrumentr_environment_t lex_env =
            instrumentr_closure_get_environment(closure);

        /* if function is being named in the same environment in which it was
         * created, then this is the canonical name. */
        if (lex_env == environment) {
            instrumentr_char_t charval = instrumentr_symbol_get_element(symbol);
            instrumentr_closure_set_name(closure,
                                         instrumentr_char_get_element(charval));
        }
    }
}

void instrumentr_trace_variable_definition(dyntracer_t* dyntracer,
                                           const SEXP r_symbol,
                                           const SEXP r_value,
                                           const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_DEFINITION;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, 1);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    instrumentr_environment_set_last_write_time(
        environment, instrumentr_state_get_time(state));

    set_function_name(symbol, value, environment);

    TRACING_INVOKE_CALLBACK(event,
                            variable_definition_function_t,
                            symbol,
                            symbol,
                            value,
                            value,
                            environment,
                            environment);

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

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, 1);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    instrumentr_environment_set_last_write_time(
        environment, instrumentr_state_get_time(state));

    set_function_name(symbol, value, environment);

    TRACING_INVOKE_CALLBACK(event,
                            variable_definition_function_t,
                            symbol,
                            symbol,
                            value,
                            value,
                            environment,
                            environment);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_removal(dyntracer_t* dyntracer,
                                        const SEXP r_symbol,
                                        const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_REMOVAL;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    instrumentr_environment_set_last_write_time(
        environment, instrumentr_state_get_time(state));

    TRACING_INVOKE_CALLBACK(event,
                            variable_removal_function_t,
                            symbol,
                            symbol,
                            environment,
                            environment);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_variable_lookup(dyntracer_t* dyntracer,
                                       const SEXP r_symbol,
                                       const SEXP r_value,
                                       const SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VARIABLE_LOOKUP;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, 1);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    instrumentr_environment_set_last_read_time(
        environment, instrumentr_state_get_time(state));

    set_function_name(symbol, value, environment);

    TRACING_INVOKE_CALLBACK(event,
                            variable_lookup_function_t,
                            symbol,
                            symbol,
                            value,
                            value,
                            environment,
                            environment);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_function_context_lookup(dyntracer_t* dyntracer,
                                               SEXP r_symbol,
                                               SEXP r_value,
                                               SEXP r_rho) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_FUNCTION_CONTEXT_LOOKUP;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    TRACING_INITIALIZE(event)

    instrumentr_value_t symval =
        instrumentr_state_value_table_lookup(state, r_symbol, 1);
    instrumentr_symbol_t symbol = instrumentr_value_as_symbol(symval);

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_value, 1);

    instrumentr_value_t envval =
        instrumentr_state_value_table_lookup(state, r_rho, 1);
    instrumentr_environment_t environment =
        instrumentr_value_as_environment(envval);

    TRACING_INVOKE_CALLBACK(event,
                            function_context_lookup_function_t,
                            symbol,
                            symbol,
                            value,
                            value,
                            environment,
                            environment)

    TRACING_FINALIZE(event)
}

void instrumentr_trace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_GC_ALLOCATION;

    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_value_t value = NULL;

    TRACING_INITIALIZE(event)

    value = instrumentr_state_value_table_insert(state, r_object);

    TRACING_INVOKE_CALLBACK(event, gc_allocation_function_t, value, value);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_multivalue_finalize(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    std::vector<instrumentr_value_t> values =
        instrumentr_state_value_table_get_values(state);

    for (instrumentr_value_t value: values) {
        instrumentr_trace_value_finalize(
            tracer, instrumentr_value_get_sexp(value), value);
    }
}

instrumentr_value_t
instrumentr_trace_value_finalize(instrumentr_tracer_t tracer,
                                 SEXP r_object,
                                 instrumentr_value_t value) {
    instrumentr_event_t event = INSTRUMENTR_EVENT_VALUE_FINALIZE;

    TRACING_INITIALIZE(event)

    if (value == NULL) {
        value = instrumentr_state_value_table_get(state, r_object);
    }

    if (value != NULL) {
        if (instrumentr_value_is_closure(value) ||
            instrumentr_value_is_environment(value)) {
            TRACING_INVOKE_CALLBACK(
                event, value_finalize_function_t, value, value);
        }
    }

    TRACING_FINALIZE(event)

    return value;
}

void instrumentr_trace_gc_deallocation(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_value_t value =
        instrumentr_trace_value_finalize(tracer, r_object, NULL);

    instrumentr_event_t event = INSTRUMENTR_EVENT_GC_DEALLOCATION;

    TRACING_INITIALIZE(event)

    TRACING_INVOKE_CALLBACK(event, gc_deallocation_function_t, value, value);

    if (value != NULL) {
        instrumentr_state_value_table_remove(state, r_object);
    }

    TRACING_FINALIZE(event)
}

void instrumentr_trace_error(dyntracer_t* dyntracer,
                             SEXP r_call,
                             const char* format,
                             va_list ap) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_ERROR;

    TRACING_INITIALIZE(event)

    /* TODO: should this be instrumentr_language_t? */
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_call, 1);

    /* TODO: handle format and ap arguments and remove call to R callback. */
    TRACING_INVOKE_CALLBACK(event, error_function_t, value, value);

    TRACING_FINALIZE(event)
}

void instrumentr_trace_attribute_set(dyntracer_t* dyntracer,
                                     SEXP r_object,
                                     SEXP r_name,
                                     SEXP r_value) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);

    instrumentr_event_t event = INSTRUMENTR_EVENT_ATTRIBUTE_SET;

    TRACING_INITIALIZE(event)

    instrumentr_value_t object =
        instrumentr_state_value_table_lookup(state, r_object, 1);

    instrumentr_value_t attr_name_val =
        instrumentr_state_value_table_lookup(state, r_name, 1);

    instrumentr_symbol_t attr_name = instrumentr_value_as_symbol(attr_name_val);

    instrumentr_value_t attr_value =
        instrumentr_state_value_table_lookup(state, r_value, 1);

    TRACING_INVOKE_CALLBACK(event,
                            attribute_set_function_t,
                            object,
                            value,
                            attr_name,
                            symbol,
                            attr_value,
                            value);

    TRACING_FINALIZE(event)
}
