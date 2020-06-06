#include "r_api.h"
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
                                SEXP r_invalid_value) {
    lightr::initialize_lightr(
        r_package_environment, r_state_environment, r_invalid_value);
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

SEXP r_lightr_trace_application_entry(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_entry_callback()) {
        SEXP r_application_entry_callback =
            context->get_application_entry_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationEntryCallback,
            Rf_lang3(r_application_entry_callback, r_context, r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_application_exit(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_application_exit_callback()) {
        SEXP r_application_exit_callback =
            context->get_application_exit_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::ApplicationExitCallback,
            Rf_lang3(r_application_exit_callback, r_context, r_application),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_package_entry(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_package_entry_callback()) {
        SEXP r_package_entry_callback = context->get_package_entry_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::PackageEntryCallback,
            Rf_lang4(
                r_package_entry_callback, r_context, r_application, r_package),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_package_exit(SEXP r_context,
                                 SEXP r_application,
                                 SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_package_exit_callback()) {
        SEXP r_package_exit_callback = context->get_package_exit_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::PackageExitCallback,
            Rf_lang4(
                r_package_exit_callback, r_context, r_application, r_package),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_function_entry(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package,
                                   SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_function_entry_callback()) {
        SEXP r_function_entry_callback = context->get_function_entry_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::FunctionEntryCallback,
            Rf_lang5(r_function_entry_callback,
                     r_context,
                     r_application,
                     r_package,
                     r_function),
            r_environment);
    }

    return result;
}

SEXP r_lightr_trace_function_exit(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package,
                                  SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);
    SEXP result = R_NilValue;

    if (context->has_function_exit_callback()) {
        SEXP r_function_exit_callback = context->get_function_exit_callback();
        SEXP r_environment = context->get_environment();

        result = r_lightr_execute_tracing_component(
            false,
            ExecutionContext::FunctionExitCallback,
            Rf_lang5(r_function_exit_callback,
                     r_context,
                     r_application,
                     r_package,
                     r_function),
            r_environment);
    }

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
            Rf_lang6(r_call_entry_callback,
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

        result = r_lightr_execute_tracing_component(false,
                                           ExecutionContext::CallExitCallback,
                                           Rf_lang6(r_call_exit_callback,
                                                    r_context,
                                                    r_application,
                                                    r_package,
                                                    r_function,
                                                    Call::to_sexp(call)),
                                           r_environment);
    }

    return result;
}
