#include "Rincludes.h"

#ifdef USING_DYNTRACE

#    include "rdyntrace.h"
#    include "Context.hpp"
#    include "r_context.h"
#    include <iostream>

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::GcAllocationCallback;
using instrumentr::GcAllocationCallbackSPtr;
using instrumentr::VariableAssignmentCallback;
using instrumentr::VariableAssignmentCallbackSPtr;
using instrumentr::VariableDefinitionCallback;
using instrumentr::VariableDefinitionCallbackSPtr;
using instrumentr::VariableLookupCallback;
using instrumentr::VariableLookupCallbackSPtr;
using instrumentr::VariableRemovalCallback;
using instrumentr::VariableRemovalCallbackSPtr;

inline ContextSPtr extract_context_from_dyntracer(dyntracer_t* dyntracer) {
    void* object_sptr_ptr = dyntracer_get_data(dyntracer);

    if (object_sptr_ptr == NULL) {
        Rf_error("extract_context_from_dyntracer: context is null");
    }

    ContextSPtr context = *static_cast<ContextSPtr*>(object_sptr_ptr);
    return context;
}

void dyntrace_gc_allocation(dyntracer_t* dyntracer, SEXP r_object) {
    ContextSPtr context = extract_context_from_dyntracer(dyntracer);

    if (context->is_tracing_enabled() &&
        context->has_gc_allocation_callback()) {
        GcAllocationCallbackSPtr callback =
            context->get_gc_allocation_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            ApplicationSPtr application = context->get_application();
            SEXP r_context = to_sexp(context);
            SEXP r_application = to_sexp(application);

            callback->invoke(r_context, r_application, r_object);

            context->finalize_callback_invocation();
        }
    }
}

void dyntrace_variable_definition(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    ContextSPtr context = extract_context_from_dyntracer(dyntracer);

    if (context->is_tracing_enabled() &&
        context->has_variable_definition_callback()) {
        VariableDefinitionCallbackSPtr callback =
            context->get_variable_definition_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            ApplicationSPtr application = context->get_application();
            SEXP r_application = to_sexp(application);
            SEXP r_context = to_sexp(context);

            SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

            callback->invoke(
                r_context, r_application, r_variable, r_value, r_rho);

            UNPROTECT(1);

            context->finalize_callback_invocation();
        }
    }
}

void dyntrace_variable_assignment(dyntracer_t* dyntracer,
                                  const SEXP r_symbol,
                                  const SEXP r_value,
                                  const SEXP r_rho) {
    ContextSPtr context = extract_context_from_dyntracer(dyntracer);

    if (context->is_tracing_enabled() &&
        context->has_variable_assignment_callback()) {
        VariableAssignmentCallbackSPtr callback =
            context->get_variable_assignment_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            ApplicationSPtr application = context->get_application();
            SEXP r_application = to_sexp(application);
            SEXP r_context = to_sexp(context);

            SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

            callback->invoke(
                r_context, r_application, r_variable, r_value, r_rho);

            UNPROTECT(1);

            context->finalize_callback_invocation();
        }
    }
}

void dyntrace_variable_removal(dyntracer_t* dyntracer,
                               const SEXP r_symbol,
                               const SEXP r_rho) {
    ContextSPtr context = extract_context_from_dyntracer(dyntracer);

    if (context->is_tracing_enabled() &&
        context->has_variable_removal_callback()) {
        VariableRemovalCallbackSPtr callback =
            context->get_variable_removal_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            ApplicationSPtr application = context->get_application();
            SEXP r_application = to_sexp(application);
            SEXP r_context = to_sexp(context);

            SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

            callback->invoke(r_context, r_application, r_variable, r_rho);

            UNPROTECT(1);

            context->finalize_callback_invocation();
        }
    }
}

void dyntrace_variable_lookup(dyntracer_t* dyntracer,
                              const SEXP r_symbol,
                              const SEXP r_value,
                              const SEXP r_rho) {
    ContextSPtr context = extract_context_from_dyntracer(dyntracer);

    if (context->is_tracing_enabled() &&
        context->has_variable_lookup_callback()) {
        VariableLookupCallbackSPtr callback =
            context->get_variable_lookup_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            ApplicationSPtr application = context->get_application();
            SEXP r_context = to_sexp(context);
            SEXP r_application = to_sexp(application);

            SEXP r_variable = PROTECT(mkString(CHAR(PRINTNAME(r_symbol))));

            callback->invoke(
                r_context, r_application, r_variable, r_value, r_rho);

            UNPROTECT(1);

            context->finalize_callback_invocation();
        }
    }
}

dyntracer_t* rdyntrace_create_dyntracer() {
    dyntracer_t* dyntracer = dyntracer_create(NULL);

    dyntracer_set_gc_allocate_callback(dyntracer, dyntrace_gc_allocation);
    dyntracer_set_environment_variable_define_callback(
        dyntracer, dyntrace_variable_definition);
    dyntracer_set_environment_variable_assign_callback(
        dyntracer, dyntrace_variable_assignment);
    dyntracer_set_environment_variable_remove_callback(
        dyntracer, dyntrace_variable_removal);
    dyntracer_set_environment_variable_lookup_callback(
        dyntracer, dyntrace_variable_lookup);

    return dyntracer;
}

void rdyntrace_destroy_dyntracer(dyntracer_t* dyntracer) {
    ContextSPtr* context_ptr =
        static_cast<ContextSPtr*>(dyntracer_destroy(dyntracer));
    delete context_ptr;
}

#endif /* USING_DYNTRACE */
