#include <instrumentr/types.h>
#include "instrumentr_internals.h"

#include "object.h"
#include "tracer.h"
#include "callback.h"
#include "application.h"
#include "function.h"
#include "package.h"
#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "trace.h"
#include "dyntrace.h"
#include "dyntrace.h"
#include "utilities.h"
#include "interop.h"
#include "funtab.h"
#include "context.h"
#include "frame.h"
#include "state.h"
#include "call_stack.h"
#include "promise.h"

#define WRAP(name) SEXP r_##name = PROTECT(instrumentr_##name##_wrap(name));

#define UNWRAP(name) \
    instrumentr_##name##_t name = instrumentr_##name##_unwrap(r_##name);

#define NOTRACE(CODE)                   \
    instrumentr_tracer_disable(tracer); \
    CODE;                               \
    instrumentr_tracer_reinstate(tracer);

#define INVOKE_CALLBACK(NAME, TRACER, INIT, CCALL, RCALL, FIN)               \
    TRACER;                                                                  \
    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);        \
    INIT;                                                                    \
    instrumentr_state_increment_time(state);                                 \
    if (instrumentr_tracer_has_callback(tracer, NAME)) {                     \
        instrumentr_callback_t callback =                                    \
            instrumentr_tracer_get_callback(tracer, NAME);                   \
                                                                             \
        clock_t begin = clock();                                             \
                                                                             \
        initialize_callback_invocation(tracer, callback);                    \
                                                                             \
        if (instrumentr_callback_has_c_function(callback)) {                 \
            NAME##_FUNCTION_T cfun =                                         \
                instrumentr_callback_get_c_function(callback);               \
            CCALL;                                                           \
        } else {                                                             \
            SEXP r_callback = instrumentr_callback_wrap(callback);           \
            SEXP r_state = instrumentr_state_wrap(state);                    \
            instrumentr_event_t event =                                      \
                instrumentr_callback_get_event(callback);                    \
            const char* name = instrumentr_event_to_string(event);           \
            SEXP r_name = Rf_install(name);                                  \
            SEXP r_environment = instrumentr_tracer_get_environment(tracer); \
            RCALL;                                                           \
        }                                                                    \
                                                                             \
        finalize_callback_invocation(tracer);                                \
                                                                             \
        clock_t end = clock();                                               \
                                                                             \
        clock_t diff = end - begin;                                          \
                                                                             \
        UPDATE_EXEC_STATS(NAME, tracer, callback, diff);                     \
    }                                                                        \
    FIN;

#define UPDATE_EXEC_STATS(NAME, tracer, callback, time)        \
    instrumentr_exec_stats_t tracer_exec_stats =               \
        instrumentr_tracer_get_event_exec_stats(tracer, NAME); \
    instrumentr_exec_stats_update(tracer_exec_stats, time);    \
    instrumentr_exec_stats_t callback_exec_stats =             \
        instrumentr_callback_get_exec_stats(callback);         \
    instrumentr_exec_stats_update(callback_exec_stats, time);

void initialize_callback_invocation(instrumentr_tracer_t tracer,
                                    instrumentr_callback_t callback) {
    instrumentr_tracer_disable(tracer);
    instrumentr_tracer_set_active_callback(tracer, callback);
    instrumentr_callback_activate(callback);
}

void finalize_callback_invocation(instrumentr_tracer_t tracer) {
    instrumentr_callback_t callback =
        instrumentr_tracer_get_active_callback(tracer);
    instrumentr_callback_deactivate(callback);
    instrumentr_tracer_unset_active_callback(tracer);
    instrumentr_tracer_reinstate(tracer);
}

SEXP r_instrumentr_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    clock_t start = clock();

    SEXP r_result = R_NilValue;

#ifdef USING_DYNTRACE
    r_result = dyntrace_trace_code(instrumentr_tracer_get_dyntracer(tracer),
                                   r_code,
                                   r_environment)
                   .value;
#else
    r_result = Rf_eval(r_code, r_environment);
#endif /* USING_DYNTRACE */

    clock_t end = clock();

    clock_t diff = end - start;

    instrumentr_exec_stats_t tracing_exec_stats =
        instrumentr_tracer_get_tracing_exec_stats(tracer);
    instrumentr_exec_stats_update(tracing_exec_stats, diff);

    return r_result;
}

SEXP r_instrumentr_trace_tracing_entry(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    INSTRUMENTR_EVENT_TRACING_ENTRY,
                    /* TRACER  */
                    UNWRAP(tracer),
                    // instrumentr_tracer_enable(tracer),
                    /* INIT */
                    NOTRACE(UNWRAP(application));
                    instrumentr_tracer_set_application(tracer, application),
                    /* CCALL */
                    NOTRACE(cfun(tracer, callback, state, application)),
                    /* RCALL */
                    Rf_eval(Rf_lang5(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application),
                            r_environment),
                    /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_tracing_exit(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    INSTRUMENTR_EVENT_TRACING_EXIT,
                    /* TRACER  */
                    UNWRAP(tracer),
                    /* INIT */
                    ,
                    /* CCALL */
                    NOTRACE(UNWRAP(application);
                            cfun(tracer, callback, state, application)),
                    /* RCALL */
                    Rf_eval(Rf_lang5(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application),
                            r_environment),
                    /* FIN */
                    instrumentr_tracer_remove_application(tracer));

    instrumentr_state_promise_table_clear(state);
    return instrumentr_state_as_list(state);
}

SEXP r_instrumentr_trace_package_load(SEXP r_tracer,
                                      SEXP r_application,
                                      SEXP r_package) {
    INVOKE_CALLBACK(
        /* NAME */
        INSTRUMENTR_EVENT_PACKAGE_LOAD,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(application); UNWRAP(package);
                instrumentr_application_add_package(application, package)),
        /* CCALL */
        cfun(tracer, callback, state, application, package),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_package),
                r_environment),
        /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_unload(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package) {
    INVOKE_CALLBACK(
        /* NAME */
        INSTRUMENTR_EVENT_PACKAGE_UNLOAD,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT  */
        NOTRACE(UNWRAP(application); UNWRAP(package)),
        /* CCALL */
        cfun(tracer, callback, state, application, package),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_package),
                r_environment),
        /* FIN */
        NOTRACE(instrumentr_application_remove_package(application, package)));

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_attach(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package) {
    INVOKE_CALLBACK(
        /* NAME */
        INSTRUMENTR_EVENT_PACKAGE_ATTACH,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(package); instrumentr_package_attach(package)),
        /* CCALL */
        NOTRACE(UNWRAP(application);
                cfun(tracer, callback, state, application, package)),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_package),
                r_environment),
        /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_detach(SEXP r_tracer,
                                        SEXP r_application,
                                        SEXP r_package) {
    INVOKE_CALLBACK(
        /* NAME */
        INSTRUMENTR_EVENT_PACKAGE_DETACH,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(package)),
        /* CCALL  */
        NOTRACE(UNWRAP(application);
                cfun(tracer, callback, state, application, package)),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_package),
                r_environment),
        /* FIN */
        NOTRACE(instrumentr_package_detach(package)));

    return R_NilValue;
}

#ifdef USING_DYNTRACE

#    define INVOKE_CALL_ENTRY_CALLBACK(NAME)                            \
        INVOKE_CALLBACK(      /* NAME  */                               \
                        NAME, /* TRACER */                              \
                        instrumentr_tracer_t tracer =                   \
                            instrumentr_dyntracer_get_tracer(           \
                                dyntracer), /* INIT  */                 \
                        NOTRACE(instrumentr_application_t application = \
                                    instrumentr_tracer_get_application( \
                                        tracer)), /* CCALL */           \
                        cfun(tracer,                                    \
                             callback,                                  \
                             application,                               \
                             r_call,                                    \
                             r_op,                                      \
                             r_args,                                    \
                             r_rho), /* RCALL */                        \
                        NOTRACE(WRAP(tracer); WRAP(application));       \
                        Rf_eval(Rf_lang8(r_name,                        \
                                         r_tracer,                      \
                                         r_callback,                    \
                                         r_application,                 \
                                         r_call,                        \
                                         r_op,                          \
                                         r_args,                        \
                                         r_rho),                        \
                                r_environment);                         \
                        UNPROTECT(2), );

#    define INVOKE_CALL_EXIT_CALLBACK(NAME)                             \
        INVOKE_CALLBACK(      /* NAME  */                               \
                        NAME, /* TRACER */                              \
                        instrumentr_tracer_t tracer =                   \
                            instrumentr_dyntracer_get_tracer(           \
                                dyntracer), /* INIT  */                 \
                        NOTRACE(instrumentr_application_t application = \
                                    instrumentr_tracer_get_application( \
                                        tracer)), /* CCALL */           \
                        cfun(tracer,                                    \
                             callback,                                  \
                             application,                               \
                             r_call,                                    \
                             r_op,                                      \
                             r_args,                                    \
                             r_rho,                                     \
                             r_result), /* RCALL */                     \
                        NOTRACE(WRAP(tracer); WRAP(application));       \
                        Rf_eval(Rf_lang9(r_name,                        \
                                         r_tracer,                      \
                                         r_callback,                    \
                                         r_application,                 \
                                         r_call,                        \
                                         r_op,                          \
                                         r_args,                        \
                                         r_rho,                         \
                                         r_result),                     \
                                r_environment);                         \
                        UNPROTECT(2), );

void instrumentr_trace_builtin_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_frame_t frame =
                    instrumentr_frame_create_from_call(state, call);
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    instrumentr_call_stack_push_frame(call_stack, frame);
                    instrumentr_object_release(frame););
}

void instrumentr_trace_builtin_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    instrumentr_call_stack_pop_frame(call_stack););
}

void instrumentr_trace_special_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_frame_t frame =
                    instrumentr_frame_create_from_call(state, call);
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    instrumentr_call_stack_push_frame(call_stack, frame);
                    instrumentr_object_release(frame););
}

void instrumentr_trace_special_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    instrumentr_call_stack_pop_frame(call_stack););
}

void instrumentr_trace_closure_call_entry(instrumentr_tracer_t tracer,
                                          instrumentr_application_t application,
                                          instrumentr_package_t package,
                                          instrumentr_function_t function,
                                          instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_frame_t frame =
                    instrumentr_frame_create_from_call(state, call);
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    instrumentr_call_stack_push_frame(call_stack, frame);
                    instrumentr_object_release(frame););
}

void instrumentr_trace_closure_call_exit(instrumentr_tracer_t tracer,
                                         instrumentr_application_t application,
                                         instrumentr_package_t package,
                                         instrumentr_function_t function,
                                         instrumentr_call_t call) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         package,
                         function,
                         call),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(package);
                            WRAP(function);
                            WRAP(call););
                    Rf_eval(Rf_lang8(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call),
                            r_environment);
                    UNPROTECT(5),
                    instrumentr_call_stack_t call_stack =
                        instrumentr_application_get_call_stack(application);
                    int pc = instrumentr_call_get_parameter_count(call);
                    for(int i = 0; i < pc; ++i) {
                        instrumentr_parameter_t parameter = instrumentr_call_get_parameter_by_position(call, i);
                        int ac = instrumentr_parameter_get_argument_count(parameter);
                        for(int j = 0; j < ac; ++j) {
                            instrumentr_argument_t argument = instrumentr_parameter_get_argument_by_position(parameter, j);
                            instrumentr_object_kill((instrumentr_object_t)argument, state);
                        }
                        instrumentr_object_kill((instrumentr_object_t)parameter, state);
                    }
                    instrumentr_object_kill((instrumentr_object_t)call, state);
                    instrumentr_call_stack_pop_frame(call_stack);
                    /* instrumentr_object_kill((instrumentr_object_t)call, state)*/);
}

// SEXP instrumentr_trace_non_closure_call(dyntracer_t* dyntracer,
//                                        SEXP r_call,
//                                        SEXP r_op,
//                                        SEXP r_args,
//                                        SEXP r_rho) {
//    instrumentr_function_t function = instrumentr_funtab_get_function(r_op);
//    int builtin = instrumentr_function_is_builtin(function);
//    CCODE ccode = instrumentr_function_get_definition(function).ccode;
//
//    instrumentr_package_t package = NULL;
//    instrumentr_call_t call = NULL;
//    int tracer_count = instrumentr_get_tracer_count();
//
//    if (tracer_count != 0) {
//        package =
//            instrumentr_application_get_package_by_name(application, "base");
//        /* TODO  */
//        call = instrumentr_call_create_non_closure(function);
//    }
//
//    for (int i = 0; i < tracer_count; ++i) {
//        instrumentr_tracer_t tracer = instrumentr_get_tracer(i);
//        instrumentr_application_t application =
//            instrumentr_tracer_get_application(tracer);
//        instrumentr_call_stack_t call_stack =
//            instrumentr_application_get_call_stack(application);
//        instrumentr_call_activate(call);
//        instrumentr_call_stack_push(call_stack, call);
//
//        if (builtin) {
//            instrumentr_trace_builtin_call_entry(
//                tracer, application, package, function, call);
//        } else {
//            instrumentr_trace_special_call_entry(
//                tracer, application, package, function, call);
//        }
//    }
//
//    SEXP r_result = ccode(r_call, r_op, r_args, r_rho);
//
//    if (tracer_count != 0) {
//        instrumentr_call_set_result(call, r_result);
//    }
//
//    for (int i = 0; i < instrumentr_get_tracer_count(); ++i) {
//        instrumentr_tracer_t tracer = instrumentr_get_tracer(i);
//        instrumentr_application_t application =
//            instrumentr_tracer_get_application(tracer);
//        instrumentr_call_stack_t call_stack =
//            instrumentr_application_get_call_stack(application);
//        instrumentr_call_stack_pop(call_stack);
//        if (builtin) {
//            instrumentr_trace_builtin_call_exit(
//                tracer, application, package, function, call);
//        } else {
//            instrumentr_trace_special_call_exit(
//                tracer, application, package, function, call);
//        }
//    }
//
//    if (tracer_count != 0) {
//        instrumentr_call_deactivate(call);
//    }
//
//    return r_result;
//}

void instrumentr_trace_context_entry(instrumentr_tracer_t tracer,
                                     instrumentr_application_t application,
                                     instrumentr_context_t context) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_CONTEXT_ENTRY,
                    /* TRACER */,
                    /* INIT */,
                    /* CCALL */
                    cfun(tracer, callback, state, application, context),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(context););
                    Rf_eval(Rf_lang6(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_context),
                            r_environment);
                    UNPROTECT(3), );
}

void instrumentr_trace_context_exit(instrumentr_tracer_t tracer,
                                    instrumentr_application_t application,
                                    instrumentr_context_t context) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_CONTEXT_EXIT,
                    /* TRACER */,
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer, callback, state, application, context),
                    /* RCALL */
                    NOTRACE(WRAP(tracer); WRAP(application); WRAP(context););
                    Rf_eval(Rf_lang6(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_context),
                            r_environment);
                    UNPROTECT(3), );
}

void dyntrace_eval_entry(dyntracer_t* dyntracer,
                         SEXP r_expression,
                         SEXP r_rho) {
    INVOKE_CALLBACK(
        /* NAME  */
        INSTRUMENTR_EVENT_EVAL_ENTRY,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer, callback, state, application, r_expression, r_rho),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_expression,
                         r_rho),
                r_environment);
        UNPROTECT(2),
        /* FIN */);
}

void dyntrace_eval_exit(dyntracer_t* dyntracer,
                        SEXP r_expression,
                        SEXP r_rho,
                        SEXP r_result) {
    INVOKE_CALLBACK(
        /* NAME  */
        INSTRUMENTR_EVENT_EVAL_EXIT,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer,
             callback,
             state,
             application,
             r_expression,
             r_rho,
             r_result),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang8(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_expression,
                         r_rho,
                         r_result),
                r_environment);
        UNPROTECT(2),
        /* FIN */);
}

void dyntrace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {

    if(TYPEOF(r_object) == PROMSXP) {
        instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
        instrumentr_state_t state = instrumentr_tracer_get_state(tracer);
        instrumentr_state_promise_table_create(state, r_object);
    }

    INVOKE_CALLBACK(
        /* NAME */
        INSTRUMENTR_EVENT_GC_ALLOCATION,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer, callback, state, application, r_object),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(
            Rf_lang6(
                r_name, r_tracer, r_callback, r_state, r_application, r_object),
            r_environment);
        UNPROTECT(2),
        /* FIN */
    );
}

void dyntrace_variable_definition(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_VARIABLE_DEFINITION,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));
                /* TODO: move the rest to trace_variable_definition */
                instrumentr_application_function_map_update_name(state, application, r_symbol, r_value, r_rho);),
        cfun(tracer, callback, state, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang8(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_variable,
                         r_value,
                         r_rho),
                r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

void dyntrace_variable_assignment(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));
                /* TODO: move the rest to trace_variable_definition */
                instrumentr_application_function_map_update_name(
                                                                 state, application, r_symbol, r_value, r_rho);),
        cfun(tracer, callback, state, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang8(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_variable,
                         r_value,
                         r_rho),
                r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

void dyntrace_variable_removal(dyntracer_t* dyntracer,
                               const SEXP r_symbol,
                               const SEXP r_rho) {
    /* TODO: remove function definition in this case */
    INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_VARIABLE_REMOVAL,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, state, application, r_variable, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_variable,
                         r_rho),
                r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

void dyntrace_variable_lookup(dyntracer_t* dyntracer,
                              const SEXP r_symbol,
                              const SEXP r_value,
                              const SEXP r_rho) {
    INVOKE_CALLBACK(
        INSTRUMENTR_EVENT_VARIABLE_LOOKUP,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, state, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang8(r_name,
                         r_tracer,
                         r_callback,
                         r_state,
                         r_application,
                         r_variable,
                         r_value,
                         r_rho),
                r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

#endif

void instrumentr_trace_promise_force_entry(instrumentr_tracer_t tracer,
                                           instrumentr_promise_t promise) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_PROMISE_FORCE_ENTRY,
                    /* TRACER */
                    NOTRACE(instrumentr_application_t application =
                            instrumentr_tracer_get_application(tracer)),
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         promise),
                    /* RCALL */
                    NOTRACE(WRAP(tracer);
                            WRAP(application);
                            WRAP(promise););
                    Rf_eval(Rf_lang6(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_promise),
                            r_environment);
                    UNPROTECT(3),
                    /* FIN */);
}


void instrumentr_trace_promise_force_exit(instrumentr_tracer_t tracer,
                                          instrumentr_promise_t promise) {
    INVOKE_CALLBACK(/* NAME  */
                    INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT,
                    /* TRACER */
                    NOTRACE(instrumentr_application_t application =
                            instrumentr_tracer_get_application(tracer)),
                    /* INIT  */,
                    /* CCALL */
                    cfun(tracer,
                         callback,
                         state,
                         application,
                         promise),
                    /* RCALL */
                    NOTRACE(WRAP(tracer);
                            WRAP(application);
                            WRAP(promise););
                    Rf_eval(Rf_lang6(r_name,
                                     r_tracer,
                                     r_callback,
                                     r_state,
                                     r_application,
                                     r_promise),
                            r_environment);
                    UNPROTECT(3),
                    /* FIN */);
}

#undef INVOKE_CALLBACK
#undef UNWRAP
#undef WRAP
