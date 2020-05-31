#include "../inst/include/Context.hpp"
#include "r_api.h"

using lightr::Context;
using lightr::ContextSPtr;

SEXP r_context_create_context(SEXP r_initializer,
                              SEXP r_finalizer,
                              SEXP r_package_entry_callback,
                              SEXP r_package_exit_callback,
                              SEXP r_call_entry_callback,
                              SEXP r_call_exit_callback,
                              SEXP r_environment) {
    ContextSPtr context = std::make_shared<Context>();

    context->set_initializer(r_initializer);
    context->set_finalizer(r_finalizer);
    context->set_package_entry_callback(r_package_entry_callback);
    context->set_package_exit_callback(r_package_exit_callback);
    context->set_call_entry_callback(r_call_entry_callback);
    context->set_call_exit_callback(r_call_exit_callback);
    context->set_environment(r_environment);

    return Context::to_sexp(context);
}

SEXP r_context_set_initializer(SEXP r_context, SEXP r_initializer) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_initializer(r_initializer);
    return R_NilValue;
}

SEXP r_context_get_initializer(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_initializer();
}

SEXP r_context_set_finalizer(SEXP r_context, SEXP r_finalizer) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_finalizer(r_finalizer);
    return R_NilValue;
}

SEXP r_context_get_finalizer(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_finalizer();
}

SEXP r_context_set_package_entry_callback(SEXP r_context,
                                          SEXP r_package_entry_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_package_entry_callback(r_package_entry_callback);
    return R_NilValue;
}

SEXP r_context_get_package_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_package_entry_callback();
}

SEXP r_context_set_package_exit_callback(SEXP r_context,
                                         SEXP r_package_exit_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_package_exit_callback(r_package_exit_callback);
    return R_NilValue;
}

SEXP r_context_get_package_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_package_exit_callback();
}

SEXP r_context_set_call_entry_callback(SEXP r_context,
                                       SEXP r_call_entry_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_call_entry_callback(r_call_entry_callback);
    return R_NilValue;
}

SEXP r_context_get_call_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_call_entry_callback();
}

SEXP r_context_set_call_exit_callback(SEXP r_context,
                                      SEXP r_call_exit_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_call_exit_callback(r_call_exit_callback);
    return R_NilValue;
}

SEXP r_context_get_call_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_call_exit_callback();
}

SEXP r_context_set_environment(SEXP r_context, SEXP r_environment) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_environment(r_environment);
    return R_NilValue;
}

SEXP r_context_get_environment(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_environment();
}
