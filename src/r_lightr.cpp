#include "r_api.h"
#include "utilities.h"
#include "../inst/include/lightr.hpp"
#include "../inst/include/Context.hpp"
#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Call.hpp"

using lightr::Application;
using lightr::ApplicationSPtr;
using lightr::Call;
using lightr::CallSPtr;
using lightr::CallStack;
using lightr::CallStackSPtr;
using lightr::Context;
using lightr::ContextSPtr;
using lightr::ExecutionContext;
using lightr::Function;
using lightr::FunctionSPtr;
using lightr::Package;
using lightr::PackageSPtr;

SEXP r_lightr_is_tracing_enabled() {
    return ScalarLogical(lightr::is_tracing_enabled());
}

SEXP r_lightr_disable_tracing() {
    lightr::disable_tracing();
    return R_NilValue;
}

SEXP r_lightr_enable_tracing() {
    lightr::enable_tracing();
    return R_NilValue;
}

SEXP r_lightr_reinstate_tracing() {
    lightr::reinstate_tracing();
    return R_NilValue;
}

SEXP r_lightr_initialize_lightr(SEXP r_package_environment,
                                SEXP r_state_environment,
                                SEXP r_undefined_object) {
    lightr::initialize_lightr(
        r_package_environment, r_state_environment, r_undefined_object);
    return R_NilValue;
}

SEXP r_lightr_peek_execution_context() {
    std::string execution_context =
        lightr::execution_context_to_string(lightr::peek_execution_context());
    return mkString(execution_context.c_str());
}

SEXP r_lightr_initialize_tracing() {
    lightr::push_execution_context(ExecutionContext::Lightr);
    return R_NilValue;
}

SEXP r_lightr_finalize_tracing() {
    lightr::clear_execution_context();
    lightr::clear_tracing();
    return R_NilValue;
}

SEXP r_lightr_is_undefined_object(SEXP object) {
    return ScalarLogical(lightr::is_undefined_object(object));
}

SEXP r_lightr_is_defined_object(SEXP object) {
    return ScalarLogical(lightr::is_defined_object(object));
}

SEXP r_lightr_execute_tracing_component(bool tracing_status,
                                        ExecutionContext execution_context,
                                        SEXP r_code,
                                        SEXP r_environment) {
    lightr::set_tracing_status(tracing_status);
    lightr::push_execution_context(execution_context);

    SEXP result = Rf_eval(r_code, r_environment);

    lightr::pop_execution_context();
    lightr::reinstate_tracing();

    return result;
}

SEXP r_lightr_trace_code(SEXP r_code, SEXP r_environment) {
    return r_lightr_execute_tracing_component(
        true, ExecutionContext::Application, r_code, r_environment);
}

SEXP r_lightr_trace_application_load(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_load_callback()) {
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationLoadCallback,
            Rf_lang3(lightr::ApplicationLoadCallbackSymbol,
                     r_context,
                     r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_application_unload(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_unload_callback()) {
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationUnloadCallback,
            Rf_lang3(lightr::ApplicationUnloadCallbackSymbol,
                     r_context,
                     r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_application_attach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_attach_callback()) {
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationAttachCallback,
            Rf_lang3(lightr::ApplicationAttachCallbackSymbol,
                     r_context,
                     r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_application_detach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_detach_callback()) {
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationDetachCallback,
            Rf_lang3(lightr::ApplicationDetachCallbackSymbol,
                     r_context,
                     r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_package(ExecutionContext execution_context,
                            SEXP r_callback_symbol,
                            SEXP r_context,
                            SEXP r_application,
                            SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    SEXP r_environment = context->get_environment();
    SEXP result = r_lightr_execute_tracing_component(
        false,
        execution_context,
        Rf_lang4(r_callback_symbol, r_context, r_application, r_package),
        r_environment);

    return result;
}

SEXP r_lightr_trace_package_load(SEXP r_context,
                                 SEXP r_application,
                                 SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    application->add_package(package);

    SEXP result = R_NilValue;

    if (context->has_package_load_callback()) {
        result = r_lightr_trace_package(ExecutionContext::PackageLoadCallback,
                                        lightr::PackageLoadCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_lightr_trace_package_unload(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);

    SEXP result = R_NilValue;

    if (context->has_package_unload_callback()) {
        result = r_lightr_trace_package(ExecutionContext::PackageUnloadCallback,
                                        lightr::PackageUnloadCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    application->remove_package(package);

    return result;
}

SEXP r_lightr_trace_package_attach(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    SEXP result = R_NilValue;

    if (context->has_package_attach_callback()) {
        result = r_lightr_trace_package(ExecutionContext::PackageAttachCallback,
                                        lightr::PackageAttachCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_lightr_trace_package_detach(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    SEXP result = R_NilValue;

    if (context->has_package_detach_callback()) {
        result = r_lightr_trace_package(ExecutionContext::PackageDetachCallback,
                                        lightr::PackageDetachCallbackSymbol,
                                        r_context,
                                        r_application,
                                        r_package);
    }

    return result;
}

SEXP r_lightr_trace_function(ExecutionContext execution_context,
                             SEXP r_callback_symbol,
                             SEXP r_context,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP r_environment = context->get_environment();

    SEXP result = r_lightr_execute_tracing_component(
        false,
        execution_context,
        Rf_lang5(
            r_callback_symbol, r_context, r_application, r_package, r_function),
        r_environment);

    return result;
}

SEXP r_lightr_trace_function_attach(SEXP r_context,
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
            r_lightr_trace_function(ExecutionContext::FunctionAttachCallback,
                                    lightr::FunctionAttachCallbackSymbol,
                                    r_context,
                                    r_application,
                                    r_package,
                                    r_function);
    }

    return result;
}

SEXP r_lightr_trace_function_detach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);

    SEXP result = R_NilValue;

    if (context->has_function_detach_callback()) {
        result =
            r_lightr_trace_function(ExecutionContext::FunctionDetachCallback,
                                    lightr::FunctionDetachCallbackSymbol,
                                    r_context,
                                    r_application,
                                    r_package,
                                    r_function);
    }

    package->remove_function(function);

    return result;
}

SEXP r_lightr_trace_call_entry(SEXP r_context,
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
        SEXP r_call_entry_callback = context->get_call_entry_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::CallEntryCallback,
            Rf_lang6(CallEntryCallbackSymbol,
                     r_context,
                     r_application,
                     r_package,
                     r_function,
                     r_call),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_call_exit(SEXP r_context,
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
        SEXP r_call_exit_callback = context->get_call_exit_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::CallExitCallback,
            Rf_lang6(CallExitCallbackSymbol,
                     r_context,
                     r_application,
                     r_package,
                     r_function,
                     Call::to_sexp(call)),
            r_environment);
    }

    return result;
}
