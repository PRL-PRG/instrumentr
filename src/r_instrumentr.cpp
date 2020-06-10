#include "r_api.h"
#include "utilities.h"
#include "../inst/include/instrumentr.hpp"
#include "../inst/include/Context.hpp"
#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Call.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Call;
using instrumentr::CallSPtr;
using instrumentr::CallStack;
using instrumentr::CallStackSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::ExecutionContext;
using instrumentr::Function;
using instrumentr::FunctionSPtr;
using instrumentr::Package;
using instrumentr::PackageSPtr;

SEXP r_instrumentr_is_tracing_enabled() {
    return ScalarLogical(instrumentr::is_tracing_enabled());
}

SEXP r_instrumentr_disable_tracing() {
    instrumentr::disable_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_enable_tracing() {
    instrumentr::enable_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_reinstate_tracing() {
    instrumentr::reinstate_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_initialize_instrumentr(SEXP r_package_environment,
                                SEXP r_state_environment,
                                SEXP r_undefined_object) {
    instrumentr::initialize_instrumentr(
        r_package_environment, r_state_environment, r_undefined_object);
    return R_NilValue;
}

SEXP r_instrumentr_peek_execution_context() {
    std::string execution_context =
        instrumentr::execution_context_to_string(instrumentr::peek_execution_context());
    return mkString(execution_context.c_str());
}

SEXP r_instrumentr_initialize_tracing() {
    instrumentr::push_execution_context(ExecutionContext::Instrumentr);
    return R_NilValue;
}

SEXP r_instrumentr_finalize_tracing() {
    instrumentr::clear_execution_context();
    instrumentr::clear_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_is_undefined_object(SEXP object) {
    return ScalarLogical(instrumentr::is_undefined_object(object));
}

SEXP r_instrumentr_is_defined_object(SEXP object) {
    return ScalarLogical(instrumentr::is_defined_object(object));
}

SEXP r_instrumentr_trace(bool tracing_status,
                    ExecutionContext execution_context,
                    SEXP r_code,
                    SEXP r_environment) {
    instrumentr::set_tracing_status(tracing_status);
    instrumentr::push_execution_context(execution_context);

    SEXP result = Rf_eval(r_code, r_environment);

    instrumentr::pop_execution_context();
    instrumentr::reinstate_tracing();

    return result;
}

SEXP r_instrumentr_trace_code(SEXP r_code, SEXP r_environment) {
    return r_instrumentr_trace(
        true, ExecutionContext::Application, r_code, r_environment);
}

SEXP r_instrumentr_trace_application(ExecutionContext execution_context,
                                SEXP r_callback_symbol,
                                SEXP r_context,
                                SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP r_environment = context->get_environment();

    SEXP result =
        r_instrumentr_trace(false,
                       execution_context,
                       Rf_lang3(r_callback_symbol, r_context, r_application),
                       r_environment);

    return result;
}

SEXP r_instrumentr_trace_application_load(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_load_callback()) {
        SEXP result = r_instrumentr_trace_application(
            ExecutionContext::ApplicationLoadCallback,
            instrumentr::ApplicationLoadCallbackSymbol,
            r_context,
            r_application);
    }

    return result;
}

SEXP r_instrumentr_trace_application_unload(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_unload_callback()) {
        SEXP result = r_instrumentr_trace_application(
            ExecutionContext::ApplicationUnloadCallback,
            instrumentr::ApplicationUnloadCallbackSymbol,
            r_context,
            r_application);
    }

    return result;
}

SEXP r_instrumentr_trace_application_attach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_attach_callback()) {
        SEXP result = r_instrumentr_trace_application(
            ExecutionContext::ApplicationAttachCallback,
            instrumentr::ApplicationAttachCallbackSymbol,
            r_context,
            r_application);
    }

    return result;
}

SEXP r_instrumentr_trace_application_detach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_detach_callback()) {
        SEXP result = r_instrumentr_trace_application(
            ExecutionContext::ApplicationDetachCallback,
            instrumentr::ApplicationDetachCallbackSymbol,
            r_context,
            r_application);
    }

    return result;
}

SEXP r_instrumentr_trace_package(ExecutionContext execution_context,
                            SEXP r_callback_symbol,
                            SEXP r_context,
                            SEXP r_application,
                            SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    SEXP r_environment = context->get_environment();
    SEXP result = r_instrumentr_trace(
        false,
        execution_context,
        Rf_lang4(r_callback_symbol, r_context, r_application, r_package),
        r_environment);

    return result;
}

SEXP r_instrumentr_trace_package_load(SEXP r_context,
                                 SEXP r_application,
                                 SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    application->add_package(package);

    SEXP result = R_NilValue;

    if (context->has_package_load_callback()) {
        result = r_instrumentr_trace_package(ExecutionContext::PackageLoadCallback,
                                        instrumentr::PackageLoadCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_instrumentr_trace_package_unload(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    SEXP result = R_NilValue;

    if (context->has_package_unload_callback()) {
        result = r_instrumentr_trace_package(ExecutionContext::PackageUnloadCallback,
                                        instrumentr::PackageUnloadCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    application->remove_package(package);

    return result;
}

SEXP r_instrumentr_trace_package_attach(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    SEXP result = R_NilValue;

    if (context->has_package_attach_callback()) {
        result = r_instrumentr_trace_package(ExecutionContext::PackageAttachCallback,
                                        instrumentr::PackageAttachCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_instrumentr_trace_package_detach(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    SEXP result = R_NilValue;

    if (context->has_package_detach_callback()) {
        result = r_instrumentr_trace_package(ExecutionContext::PackageDetachCallback,
                                        instrumentr::PackageDetachCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_instrumentr_trace_function(ExecutionContext execution_context,
                             SEXP r_callback_symbol,
                             SEXP r_context,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP r_environment = context->get_environment();

    SEXP result = r_instrumentr_trace(
        false,
        execution_context,
        Rf_lang5(
            r_callback_symbol, r_context, r_application, r_package, r_function),
        r_environment);

    return result;
}

SEXP r_instrumentr_trace_function_attach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);

    package->add_function(function);

    SEXP result = R_NilValue;

    if (context->has_function_attach_callback()) {
        result =
            r_instrumentr_trace_function(ExecutionContext::FunctionAttachCallback,
                                    instrumentr::FunctionAttachCallbackSymbol,
                                    r_context,
                                    r_application,
                                    r_package,
                                    r_function);
    }

    return result;
}

SEXP r_instrumentr_trace_function_detach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);

    SEXP result = R_NilValue;

    if (context->has_function_detach_callback()) {
        result =
            r_instrumentr_trace_function(ExecutionContext::FunctionDetachCallback,
                                    instrumentr::FunctionDetachCallbackSymbol,
                                    r_context,
                                    r_application,
                                    r_package,
                                    r_function);
    }

    package->remove_function(function);

    return result;
}

SEXP r_instrumentr_trace_call(ExecutionContext execution_context,
                         SEXP r_callback_symbol,
                         SEXP r_context,
                         SEXP r_application,
                         SEXP r_package,
                         SEXP r_function,
                         SEXP r_call) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP r_environment = context->get_environment();

    SEXP result = r_instrumentr_trace(false,
                                 execution_context,
                                 Rf_lang6(r_callback_symbol,
                                          r_context,
                                          r_application,
                                          r_package,
                                          r_function,
                                          r_call),
                                 r_environment);

    return result;
}

SEXP r_instrumentr_trace_call_entry(SEXP r_context,
                               SEXP r_application,
                               SEXP r_package,
                               SEXP r_function,
                               SEXP r_call) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    CallSPtr call = Call::from_sexp(r_call);
    CallStackSPtr call_stack = application->get_call_stack();
    SEXP result = R_NilValue;

    call->set_active();

    call_stack->push_frame(call);

    if (context->has_call_entry_callback()) {
        result = r_instrumentr_trace_call(ExecutionContext::CallEntryCallback,
                                     instrumentr::CallEntryCallbackSymbol,
                                     r_context,
                                     r_application,
                                     r_package,
                                     r_function,
                                     r_call);
    }

    return result;
}

SEXP r_instrumentr_trace_call_exit(SEXP r_context,
                              SEXP r_application,
                              SEXP r_package,
                              SEXP r_function,
                              SEXP r_result) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);
    CallStackSPtr call_stack = application->get_call_stack();
    CallSPtr call = call_stack->pop_frame();
    SEXP result = R_NilValue;

    if (call->get_function()->get_name() != function->get_name()) {
        Rf_errorcall(R_NilValue,
                     "Error: unmatched call entry and exit, " //
                     "exiting '%s::%s' but expected to exit '%s'.",
                     package->get_name(),
                     function->get_name(),
                     call->get_function()->get_name());
    }

    call->set_result(r_result);

    if (context->has_call_exit_callback()) {
        result = r_instrumentr_trace_call(ExecutionContext::CallExitCallback,
                                     instrumentr::CallExitCallbackSymbol,
                                     r_context,
                                     r_application,
                                     r_package,
                                     r_function,
                                     Call::to_sexp(call));
    }

    return result;
}
