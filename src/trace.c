#include <instrumentr/types.h>
#include "instrumentr_internals.h"

#include <instrumentr/tracer.h>
#include "tracer_internals.h"

#include <instrumentr/callback.h>
#include "callback_internals.h"

#include <instrumentr/application.h>
#include "application_internals.h"

#include <instrumentr/function.h>

#include <instrumentr/package.h>
#include "package_internals.h"

#include <instrumentr/call.h>
#include "call_internals.h"

#include <instrumentr/parameter.h>

#include <instrumentr/argument.h>

#include "trace.h"
#include "dyntrace.h"
#include "dyntracer.h"
#include "utilities.h"
#include "interop.h"

#define WRAP(name) SEXP r_##name = PROTECT(instrumentr_##name##_wrap(name));

#define UNWRAP(name) \
    instrumentr_##name##_t name = instrumentr_##name##_unwrap(r_##name);

#define NOTRACE(CODE)                   \
    instrumentr_tracer_disable(tracer); \
    CODE;                               \
    instrumentr_tracer_reinstate(tracer);

#define INVOKE_CALLBACK(NAME, TRACER, INIT, CCALL, RCALL, FIN)               \
    TRACER;                                                                  \
    INIT;                                                                    \
    if (instrumentr_tracer_has_callback_##NAME(tracer)) {                    \
        instrumentr_callback_t callback =                                    \
            instrumentr_tracer_get_callback_##NAME(tracer);                  \
        initialize_callback_invocation(tracer, callback);                    \
                                                                             \
        if (instrumentr_callback_has_c_function(callback)) {                 \
            NAME##_function_t cfun = (NAME##_function_t)(                    \
                instrumentr_callback_get_c_function(callback));              \
            CCALL;                                                           \
        } else {                                                             \
            SEXP r_callback = instrumentr_callback_wrap(callback);           \
            SEXP r_name = r_instrumentr_callback_get_name(r_callback);       \
            SEXP r_environment = instrumentr_tracer_get_environment(tracer); \
            RCALL;                                                           \
        }                                                                    \
                                                                             \
        finalize_callback_invocation(tracer);                                \
    }                                                                        \
    FIN;


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

#ifdef USING_DYNTRACE
    SEXP r_result = dyntrace_trace_code(
        instrumentr_tracer_get_dyntracer(tracer), r_code, r_environment);
#else
    SEXP r_result = Rf_eval(r_code, r_environment);
#endif /* USING_DYNTRACE */

    return r_result;
}

SEXP r_instrumentr_trace_application_load(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    application_load,
                    /* TRACER  */
                    UNWRAP(tracer),
                    //instrumentr_tracer_enable(tracer),
                    /* INIT */
                    NOTRACE(UNWRAP(application));
                    instrumentr_tracer_set_application(tracer, application),
                    /* CCALL */
                    NOTRACE(cfun(tracer, callback, application)),
                    /* RCALL */
                    Rf_eval(
                        Rf_lang4(r_name, r_tracer, r_callback, r_application),
                        r_environment),
                    /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_application_unload(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    application_unload,
                    /* TRACER  */
                    UNWRAP(tracer),
                    /* INIT */
                    ,
                    /* CCALL */
                    NOTRACE(UNWRAP(application);
                            cfun(tracer, callback, application)),
                    /* RCALL */
                    Rf_eval(
                        Rf_lang4(r_name, r_tracer, r_callback, r_application),
                        r_environment),
                    /* FIN */
                    instrumentr_tracer_remove_application(tracer));

    return R_NilValue;
}

SEXP r_instrumentr_trace_application_attach(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    application_attach,
                    /* TRACER  */
                    UNWRAP(tracer),
                    /* INIT */
                    ,
                    /* CCALL */
                    NOTRACE(UNWRAP(application);
                            cfun(tracer, callback, application)),
                    /* RCALL */
                    Rf_eval(
                        Rf_lang4(r_name, r_tracer, r_callback, r_application),
                        r_environment),
                    /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_application_detach(SEXP r_tracer, SEXP r_application) {
    INVOKE_CALLBACK(/* NAME */
                    application_detach,
                    /* TRACER  */
                    UNWRAP(tracer),
                    /* INIT */
                    ,
                    /* CCALL */
                    NOTRACE(UNWRAP(application);
                            cfun(tracer, callback, application)),
                    /* RCALL */
                    Rf_eval(
                        Rf_lang4(r_name, r_tracer, r_callback, r_application),
                        r_environment),
                    /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_package_load(SEXP r_tracer,
                                      SEXP r_application,
                                      SEXP r_package) {
    INVOKE_CALLBACK(
        /* NAME */
        package_load,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(application); UNWRAP(package);
                instrumentr_application_append_package(application, package)),
        /* CCALL */
        cfun(tracer, callback, application, package),
        /* RCALL */
        Rf_eval(
            Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package),
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
        package_unload,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT  */
        NOTRACE(UNWRAP(application); UNWRAP(package)),
        /* CCALL */
        cfun(tracer, callback, application, package),
        /* RCALL */
        Rf_eval(
            Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package),
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
        package_attach,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        ,
        /* CCALL */
        NOTRACE(UNWRAP(application); UNWRAP(package);
                cfun(tracer, callback, application, package)),
        /* RCALL */
        Rf_eval(
            Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package),
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
        package_detach,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        ,
        /* CCALL  */
        NOTRACE(UNWRAP(application); UNWRAP(package);
                cfun(tracer, callback, application, package)),
        /* RCALL */
        Rf_eval(
            Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package),
            r_environment),
        /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_function_attach(SEXP r_tracer,
                                         SEXP r_application,
                                         SEXP r_package,
                                         SEXP r_function) {
    INVOKE_CALLBACK(
        /* NAME */
        function_attach,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(package); UNWRAP(function);
                instrumentr_package_append_function(package, function)),
        /* CCALL */
        NOTRACE(UNWRAP(application));
        cfun(tracer, callback, application, package, function),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_package,
                         r_function),
                r_environment),
        /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_function_detach(SEXP r_tracer,
                                         SEXP r_application,
                                         SEXP r_package,
                                         SEXP r_function) {
    INVOKE_CALLBACK(
        /* NAME */
        function_detach,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        UNWRAP(package);
        UNWRAP(function),
        /* CCALL */
        NOTRACE(UNWRAP(application));
        cfun(tracer, callback, application, package, function),
        /* RCALL */
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_package,
                         r_function),
                r_environment),
        /* FIN */
        NOTRACE(instrumentr_package_remove_function(package, function)));

    return R_NilValue;
}

SEXP r_instrumentr_trace_call_entry(SEXP r_tracer,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function,
                                    SEXP r_call) {
    INVOKE_CALLBACK(
        /* NAME  */
        call_entry,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT  */
        NOTRACE(UNWRAP(application); UNWRAP(package); UNWRAP(function);
                UNWRAP(call);
                instrumentr_call_stack_t call_stack =
                    instrumentr_application_get_call_stack(application);
                instrumentr_call_activate(call);
                instrumentr_call_stack_push(call_stack, call)),
        /* CCALL */
        cfun(tracer, callback, application, package, function, call),
        /* RCALL */
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_package,
                         r_function,
                         r_call),
                r_environment),
        /* FIN */
    );

    return R_NilValue;
}

SEXP r_instrumentr_trace_call_exit(SEXP r_tracer,
                                   SEXP r_application,
                                   SEXP r_package,
                                   SEXP r_function,
                                   SEXP r_result) {
    INVOKE_CALLBACK(
        /* NAME */
        call_exit,
        /* TRACER */
        UNWRAP(tracer),
        /* INIT */
        NOTRACE(UNWRAP(application); UNWRAP(package); UNWRAP(function);

                instrumentr_call_stack_t call_stack =
                    instrumentr_application_get_call_stack(application);

                instrumentr_call_t call =
                    instrumentr_call_stack_peek(call_stack, 0);

                WRAP(call);

                const char* expected_function_name =
                    instrumentr_function_get_name(
                        instrumentr_call_get_function(call));

                const char* actual_function_name =
                    instrumentr_function_get_name(function);

                if (strcmp(actual_function_name, expected_function_name) != 0) {
                    UNPROTECT(1);

                    finalize_callback_invocation(tracer);

                    instrumentr_log_error(
                        "Error: unmatched call entry and exit, " //
                        "exiting '%s::%s' but expected to exit '%s'.",
                        instrumentr_package_get_name(package),
                        actual_function_name,
                        expected_function_name);
                    /* NOTE: not executed */
                    return R_NilValue;
                } instrumentr_call_set_result(call, r_result)),
        /* CCALL */
        cfun(tracer, callback, application, package, function, call),
        /* RCALL */
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_package,
                         r_function,
                         r_call),
                r_environment),
        /* FIN */
        NOTRACE(instrumentr_call_stack_pop(call_stack);
                instrumentr_call_deactivate(call);
                UNPROTECT(1);
                ))

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

void dyntrace_builtin_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    INVOKE_CALL_ENTRY_CALLBACK(builtin_call_entry);
}

void dyntrace_builtin_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    INVOKE_CALL_EXIT_CALLBACK(builtin_call_exit);
}

void dyntrace_special_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    INVOKE_CALL_ENTRY_CALLBACK(special_call_entry);
}

void dyntrace_special_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    INVOKE_CALL_EXIT_CALLBACK(special_call_exit);
}

void dyntrace_closure_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    INVOKE_CALL_ENTRY_CALLBACK(closure_call_entry);
}

void dyntrace_closure_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    INVOKE_CALL_EXIT_CALLBACK(closure_call_exit);
}

void dyntrace_eval_entry(dyntracer_t* dyntracer,
                         SEXP r_expression,
                         SEXP r_rho) {
    INVOKE_CALLBACK(
        /* NAME  */
        eval_entry,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer, callback, application, r_expression, r_rho),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang6(r_name,
                         r_tracer,
                         r_callback,
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
        eval_exit,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer, callback, application, r_expression, r_rho, r_result),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_expression,
                         r_rho,
                         r_result),
                r_environment);
        UNPROTECT(2),
        /* FIN */);
}

void dyntrace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {
    INVOKE_CALLBACK(
        /* NAME */
        gc_allocation,
        /* TRACER */
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        /* INIT  */
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer)),
        /* CCALL */
        cfun(tracer, callback, application, r_object),
        /* RCALL */
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang5(r_name, r_tracer, r_callback, r_application, r_object),
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
        variable_definition,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
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
        variable_assignment,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
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
    INVOKE_CALLBACK(
        variable_removal,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, application, r_variable, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(
            Rf_lang6(
                r_name, r_tracer, r_callback, r_application, r_variable, r_rho),
            r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

void dyntrace_variable_lookup(dyntracer_t* dyntracer,
                              const SEXP r_symbol,
                              const SEXP r_value,
                              const SEXP r_rho) {
    INVOKE_CALLBACK(
        variable_lookup,
        instrumentr_tracer_t tracer =
            instrumentr_dyntracer_get_tracer(dyntracer),
        NOTRACE(instrumentr_application_t application =
                    instrumentr_tracer_get_application(tracer);
                SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))))),
        cfun(tracer, callback, application, r_variable, r_value, r_rho),
        NOTRACE(WRAP(tracer); WRAP(application));
        Rf_eval(Rf_lang7(r_name,
                         r_tracer,
                         r_callback,
                         r_application,
                         r_variable,
                         r_value,
                         r_rho),
                r_environment);
        UNPROTECT(2), UNPROTECT(1));
}

#endif

#undef INVOKE_CALLBACK
#undef UNWRAP
#undef WRAP
