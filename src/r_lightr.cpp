#include "../inst/include/lightr.hpp"
#include "r_api.h"

using lightr::Application;
using lightr::ApplicationSPtr;
using lightr::Context;
using lightr::ContextSPtr;

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
                                SEXP r_state_environment) {
    lightr::initialize_lightr(r_package_environment, r_state_environment);
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
