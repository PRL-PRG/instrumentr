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

SEXP r_lightr_eval(SEXP r_code, SEXP r_environment) {
    return Rf_eval(r_code, r_environment);
}

SEXP r_lightr_initialize_lightr(SEXP r_package_environment,
                                SEXP r_state_environment,
                                SEXP r_invalid_value) {
    lightr::initialize_lightr(
        r_package_environment, r_state_environment, r_invalid_value);
    return R_NilValue;
}

SEXP r_lightr_trace_application_entry(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_application_entry_callback()) {
        SEXP r_application_entry_callback =
            context->get_application_entry_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(
            Rf_lang3(r_application_entry_callback, r_context, r_application),
            r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}

SEXP r_lightr_trace_application_exit(SEXP r_context, SEXP r_application) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_application_exit_callback()) {
        SEXP r_application_exit_callback =
            context->get_application_exit_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(Rf_lang3(r_application_exit_callback, r_context, r_application),
                r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}

SEXP r_lightr_trace_package_entry(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_package_entry_callback()) {
        SEXP r_package_entry_callback = context->get_package_entry_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(
            Rf_lang4(
                r_package_entry_callback, r_context, r_application, r_package),
            r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}

SEXP r_lightr_trace_package_exit(SEXP r_context,
                                 SEXP r_application,
                                 SEXP r_package) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_package_exit_callback()) {
        SEXP r_package_exit_callback = context->get_package_exit_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(
            Rf_lang4(
                r_package_exit_callback, r_context, r_application, r_package),
            r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}

SEXP r_lightr_trace_function_entry(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package,
                                   SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_function_entry_callback()) {
        SEXP r_function_entry_callback = context->get_function_entry_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(Rf_lang5(r_function_entry_callback,
                         r_context,
                         r_application,
                         r_package,
                         r_function),
                r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}

SEXP r_lightr_trace_function_exit(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package,
                                  SEXP r_function) {
    ContextSPtr context = Context::from_sexp(r_context);

    if (context->has_function_exit_callback()) {
        SEXP r_function_exit_callback = context->get_function_exit_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(Rf_lang5(r_function_exit_callback,
                         r_context,
                         r_application,
                         r_package,
                         r_function),
                r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
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

    call->set_active();

    call_stack->push_frame(call);

    if (context->has_call_entry_callback()) {
        SEXP r_call_entry_callback = context->get_call_entry_callback();
        SEXP r_environment = context->get_environment();

        lightr::disable_tracing();

        Rf_eval(Rf_lang6(r_call_entry_callback,
                         r_context,
                         r_application,
                         r_package,
                         r_function,
                         r_call),
                r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
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

        lightr::disable_tracing();

        Rf_eval(Rf_lang6(r_call_exit_callback,
                         r_context,
                         r_application,
                         r_package,
                         r_function,
                         Call::to_sexp(call)),
                r_environment);

        lightr::reinstate_tracing();
    }

    return R_NilValue;
}
