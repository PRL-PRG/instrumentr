#include <instrumentr/tracer.h>
#include "tracer_internals.h"
#include "callback_internals.h"
#include <instrumentr/application.h>
#include <instrumentr/call.h>
#include <instrumentr/function.h>
#include <instrumentr/package.h>
#include <instrumentr/parameter.h>
#include <instrumentr/argument.h>
#include "trace.h"
#include "dyntrace.h"

void instrumentr_tracer_initialize_tracing(instrumentr_tracer_t tracer) {
    instrumentr_add_tracer(tracer);
}

SEXP r_tracer_initialize_tracing(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_initialize_tracing(tracer);
    return R_NilValue;
}

void instrumentr_tracer_finalize_tracing(instrumentr_tracer_t tracer) {
    instrumentr_remove_tracer(tracer);
    instrumentr_tracer_reset(tracer);
}

SEXP r_tracer_finalize_tracing(SEXP r_tracer) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);
    instrumentr_tracer_finalize_tracing(tracer);
    return R_NilValue;
}

#define WRAP(name) SEXP r_##name = instrumentr_##name##_wrap(name);

#define UNWRAP(name) \
    instrumentr_##name##_t name = instrumentr_##name##_unwrap(r_##name);

#define INVOKE_CALLBACK(name, c_call, r_call)                          \
    if (instrumentr_tracer_is_enabled(tracer) &&                       \
        instrumentr_tracer_has_##name(tracer)) {                       \
        instrumentr_callback_t callback =                              \
            instrumentr_tracer_get_##name##_callback(tracer);          \
                                                                       \
        if (instrumentr_callback_is_enabled(callback)) {               \
            initialize_callback_invocation(tracer, callback); \
                                                                       \
            if (instrumentr_callback_has_c_function(callback)) {       \
                void* function =                                       \
                    instrumentr_callback_get_c_function(callback);     \
                c_call;                                                \
                                                                       \
            } else {                                                   \
                SEXP r_name = instrumentr_callback_get_name(callback); \
                SEXP r_environment =                                   \
                    instrumentr_tracer_get_environment(tracer);        \
                Rf_eval(r_call, r_environment);                        \
            }                                                          \
                                                                       \
            finalize_callback_invocation(tracer, callback);            \
        }                                                              \
    }

void initialize_callback_invocation(instrumentr_tracer_t tracer,
                                    instrumentr_callback_t callback) {
    instrumentr_tracer_disable(tracer);
    instrumentr_tracer_set_active_callback(tracer, callback);
    instrumentr_callback_activate(callback);
}

void finalize_callback_invocation(instrumentr_tracer_t tracer,
                                  instrumentr_callback_t callback) {
    instrumentr_callback_deactivate(callback);
    instrumentr_tracer_unset_active_callback(tracer);
    instrumentr_tracer_reinstate(tracer);
}

SEXP r_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment) {
    instrumentr_tracer_t tracer = instrumentr_tracer_unwrap(r_tracer);

    instrumentr_tracer_set_tracing_status(tracer, 1);

#ifdef USING_DYNTRACE
    SEXP r_result =
        dyntrace_trace_code(instrumentr_tracer_get_dyntracer(tracer), r_code, r_environment);
#else
    SEXP r_result = Rf_eval(r_code, r_environment);
#endif /* USING_DYNTRACE */

    return r_result;
}


SEXP r_trace_application_load(SEXP r_tracer, SEXP r_application) {
    UNWRAP(tracer);
    UNWRAP(callback);
    UNWRAP(application);

    instrumentr_tracer_set_application(tracer, application);

    INVOKE_CALLBACK(application_load,
                    function(tracer, callback, application),
                    Rf_lang4(r_name, r_tracer, r_callback, r_application));

    return R_NilValue;
}

SEXP r_trace_application_unload(SEXP r_tracer, SEXP r_application) {
    UNWRAP(tracer);
    UNWRAP(callback);
    UNWRAP(application);

    INVOKE_CALLBACK(application_unload,
                    function(tracer, callback, application),
                    Rf_lang4(r_name, r_tracer, r_callback, r_application));

    instrumentr_tracer_free_application(tracer);

    return R_NilValue;
}

SEXP r_trace_application_attach(SEXP r_tracer, SEXP r_application) {
    UNWRAP(tracer);
    UNWRAP(callback);
    UNWRAP(application);

    INVOKE_CALLBACK(application_attach,
                    function(tracer, callback, application),
                    Rf_lang4(r_name, r_tracer, r_callback, r_application));

    return R_NilValue;
}

SEXP r_trace_application_detach(SEXP r_tracer, SEXP r_application) {
    UNWRAP(tracer);
    UNWRAP(callback);
    UNWRAP(application);

    INVOKE_CALLBACK(application_detach,
                    function(tracer, callback, application),
                    Rf_lang4(r_name, r_tracer, r_callback, r_application));

    return R_NilValue;
}

SEXP r_trace_package_load(SEXP r_tracer,
                          SEXP r_application,
                          SEXP r_package) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);

    instrumentr_application_add_package(application, package);

    INVOKE_CALLBACK(
        package_load,
        function(tracer, callback, application, package),
        Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package));

    return R_NilValue;
}

SEXP r_trace_package_unload(SEXP r_tracer,
                            SEXP r_application,
                            SEXP r_package) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);

    INVOKE_CALLBACK(
        package_unload,
        function(tracer, callback, application, package),
        Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package));

    instrumentr_application_remove_package(application, package);

    return R_NilValue;
}

SEXP r_trace_package_attach(SEXP r_tracer,
                            SEXP r_application,
                            SEXP r_package) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);

    INVOKE_CALLBACK(
        package_attach,
        function(tracer, callback, application, package),
        Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package));

    return R_NilValue;
}

SEXP r_trace_package_detach(SEXP r_tracer,
                            SEXP r_application,
                            SEXP r_package) {
    UNWRAP(tracer);
    UNWRAP(callback);
    UNWRAP(application);
    UNWRAP(package);

    INVOKE_CALLBACK(
        package_detach,
        function(tracer, callback, application, package),
        Rf_lang5(r_name, r_tracer, r_callback, r_application, r_package));

    return R_NilValue;
}

SEXP r_trace_function_attach(SEXP r_tracer,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);
    UNWRAP(function);

    instrumentr_package_add_function(package, function);

    INVOKE_CALLBACK(function_attach,
                    function(tracer, callback, application, package, function),
                    Rf_lang6(r_name,
                             r_tracer,
                             r_callback,
                             r_application,
                             r_package,
                             r_function));

    return R_NilValue;
}

SEXP r_trace_function_detach(SEXP r_tracer,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);
    UNWRAP(function);

    INVOKE_CALLBACK(function_detach,
                    function(tracer, callback, application, package, function),
                    Rf_lang6(r_name,
                             r_tracer,
                             r_callback,
                             r_application,
                             r_package,
                             r_function));

    instrumentr_package_remove_function(package, function);

    return R_NilValue;
}

SEXP r_trace_call_entry(SEXP r_tracer,
                        SEXP r_application,
                        SEXP r_package,
                        SEXP r_function,
                        SEXP r_call) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);
    UNWRAP(function);
    UNWRAP(call);

    instrumentr_call_stack_t call_stack =
        instrumentr_application_get_call_stack(application);

    instrumentr_call_set_active(call);

    instrumentr_call_stack_push(call);

    INVOKE_CALLBACK(
        call_entry,
        function(tracer, callback, application, package, function, call),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_package,
                 r_function,
                 r_call));

    return R_NilValue;
}

SEXP r_trace_call_exit(SEXP r_tracer,
                       SEXP r_application,
                       SEXP r_package,
                       SEXP r_function,
                       SEXP r_result) {
    UNWRAP(tracer);
    UNWRAP(application);
    UNWRAP(package);
    UNWRAP(function);

    instrumentr_call_stack_t call_stack =
        instrumentr_application_get_call_stack(application);
    instrumentr_call_t call = instrumentr_call_stack_peek(0);

    WRAP(call);

    const char* expected_function_name =
        instrumentr_function_get_name(instrumentr_call_get_function(call));
    const char* actual_function_name = instrumentr_function_get_name(function);

    if (strcmp(actual_function_name, expected_function_name) != 0) {
        UNPROTECT(1);

        instrumentr_raise_error("Error: unmatched call entry and exit, " //
                                "exiting '%s::%s' but expected to exit '%s'.",
                                instrumentr_package_get_name(),
                                actual_function_name,
                                expected_function_name);
    }

    instrumentr_call_set_result(call, r_result);

    INVOKE_CALLBACK(
        call_exit,
        function(tracer, callback, application, package, function, call),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_package,
                 r_function,
                 r_call));

    instrumentr_call_stack_pop(call_stack);

    UNPROTECT(1);

    return R_NilValue;
}

#ifdef USING_DYNTRACE

void dyntrace_builtin_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(
        builtin_call_entry,
        function(tracer, callback, application, r_call, r_op, r_args, r_rho),
        Rf_lang8(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_call,
                 r_op,
                 r_args,
                 r_rho));

    UNPROTECT(2);
}

void dyntrace_builtin_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(builtin_call_exit,
                    function(tracer,
                             callback,
                             application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result),
                    Rf_lang9(r_name,
                             r_tracer,
                             r_callback,
                             r_application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result));

    UNPROTECT(2);
}

void dyntrace_special_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(
        special_call_entry,
        function(tracer, callback, application, r_call, r_op, r_args, r_rho),
        Rf_lang8(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_call,
                 r_op,
                 r_args,
                 r_rho));

    UNPROTECT(2);
}

void dyntrace_special_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(special_call_exit,
                    function(tracer,
                             callback,
                             application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result),
                    Rf_lang9(r_name,
                             r_tracer,
                             r_callback,
                             r_application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result));

    UNPROTECT(2);
}

void dyntrace_closure_call_entry(dyntracer_t* dyntracer,
                                 SEXP r_call,
                                 SEXP r_op,
                                 SEXP r_args,
                                 SEXP r_rho,
                                 dyntrace_dispatch_t dispatch) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(
        closure_call_entry,
        function(tracer, callback, application, r_call, r_op, r_args, r_rho),
        Rf_lang8(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_call,
                 r_op,
                 r_args,
                 r_rho));

    UNPROTECT(2);
}

void dyntrace_closure_call_exit(dyntracer_t* dyntracer,
                                SEXP r_call,
                                SEXP r_op,
                                SEXP r_args,
                                SEXP r_rho,
                                dyntrace_dispatch_t dispatch,
                                SEXP r_result) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(closure_call_exit,
                    function(tracer,
                             callback,
                             application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result),
                    Rf_lang9(r_name,
                             r_tracer,
                             r_callback,
                             r_application,
                             r_call,
                             r_op,
                             r_args,
                             r_rho,
                             r_result));

    UNPROTECT(2);
}

void dyntrace_eval_entry(dyntracer_t* dyntracer,
                         SEXP r_expression,
                         SEXP r_rho) {
    instrumentr_tracer_t tracer = extract_tracer_from_dyntracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(
        eval_entry,
        function(tracer, callback, application, r_expression, r_rho),
        Rf_lang6(
            r_name, r_tracer, r_callback, r_application, r_expression, r_rho));

    UNPROTECT(2);
}

void dyntrace_eval_exit(dyntracer_t* dyntracer,
                        SEXP r_expression,
                        SEXP r_rho,
                        SEXP r_result) {
    instrumentr_tracer_t tracer = extract_tracer_from_dyntracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    INVOKE_CALLBACK(
        eval_exit,
        function(tracer, callback, application, r_expression, r_rho, r_result),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_expression,
                 r_rho,
                 r_result));

    UNPROTECT(2);
}

void dyntrace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

    INVOKE_CALLBACK(
        gc_allocation,
        function(tracer, callback, application, r_object),
        Rf_lang7(r_name, r_tracer, r_callback, r_application, r_object));

    UNPROTECT(2);
}

void dyntrace_variable_definition(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

    INVOKE_CALLBACK(
        variable_definition,
        function(tracer, callback, application, r_variable, r_value, r_rho),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_variable,
                 r_value,
                 r_rho));

    UNPROTECT(3);
}

void dyntrace_variable_assignment(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

    INVOKE_CALLBACK(
        variable_assignment,
        function(tracer, callback, application, r_variable, r_value, r_rho),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_variable,
                 r_value,
                 r_rho));

    UNPROTECT(3);
}

void dyntrace_variable_removal(dyntracer_t* dyntracer,
                               const SEXP r_symbol,
                               const SEXP r_rho) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

    INVOKE_CALLBACK(
        variable_removal,
        function(tracer, callback, application, r_variable, r_rho),
        Rf_lang6(
            r_name, r_tracer, r_callback, r_application, r_variable, r_rho));

    UNPROTECT(3);
}

void dyntrace_variable_lookup(dyntracer_t* dyntracer,
                              const SEXP r_symbol,
                              const SEXP r_value,
                              const SEXP r_rho) {
    instrumentr_tracer_t tracer = instrumentr_dyntracer_get_tracer(dyntracer);
    instrumentr_application_t application =
        instrumentr_tracer_get_application(tracer);

    WRAP(tracer);
    WRAP(application);

    SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

    INVOKE_CALLBACK(
        variable_lookup,
        function(tracer, callback, application, r_variable, r_value, r_rho),
        Rf_lang7(r_name,
                 r_tracer,
                 r_callback,
                 r_application,
                 r_variable,
                 r_value,
                 r_rho));

    UNPROTECT(3);
}

#endif

#undef INVOKE_CALLBACK
#undef UNWRAP
#undef WRAP
