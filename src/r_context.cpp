#include "../inst/include/Context.hpp"
#include "r_api.h"

using lightr::Context;
using lightr::ContextSPtr;

SEXP r_context_create_context(SEXP r_environment) {
    ContextSPtr context = std::make_shared<Context>(r_environment);
    return Context::to_sexp(context);
}

SEXP r_context_set_application_entry_callback(
    SEXP r_context,
    SEXP r_application_entry_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_application_entry_callback(r_application_entry_callback);
    return R_NilValue;
}

SEXP r_context_get_application_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_application_entry_callback();
}

SEXP r_context_has_application_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_application_entry_callback());
}

SEXP r_context_set_application_exit_callback(SEXP r_context,
                                             SEXP r_application_exit_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_application_exit_callback(r_application_exit_callback);
    return R_NilValue;
}

SEXP r_context_get_application_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_application_exit_callback();
}

SEXP r_context_has_application_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_application_exit_callback());
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

SEXP r_context_has_package_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_package_entry_callback());
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

SEXP r_context_has_package_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_package_exit_callback());
}

SEXP r_context_set_function_entry_callback(SEXP r_context,
                                           SEXP r_function_entry_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_function_entry_callback(r_function_entry_callback);
    return R_NilValue;
}

SEXP r_context_get_function_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_function_entry_callback();
}

SEXP r_context_has_function_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_function_entry_callback());
}

SEXP r_context_set_function_exit_callback(SEXP r_context,
                                          SEXP r_function_exit_callback) {
    ContextSPtr context = Context::from_sexp(r_context);
    context->set_function_exit_callback(r_function_exit_callback);
    return R_NilValue;
}

SEXP r_context_get_function_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return context->get_function_exit_callback();
}

SEXP r_context_has_function_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_function_exit_callback());
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

SEXP r_context_has_call_entry_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_call_entry_callback());
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

SEXP r_context_has_call_exit_callback(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    return ScalarLogical(context->has_call_exit_callback());
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

SEXP r_context_trace_package(SEXP r_context, SEXP r_package_name) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    context->trace_package(package_name);
    return R_NilValue;
}

SEXP r_context_trace_function(SEXP r_context,
                              SEXP r_package_name,
                              SEXP r_function_name) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    const std::string function_name(CHAR(asChar(r_function_name)));
    context->trace_function(package_name, function_name);
    return R_NilValue;
}

SEXP r_context_get_traced_packages(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::vector<std::string> packages = context->get_traced_packages();
    int package_count = packages.size();
    SEXP r_packages = PROTECT(allocVector(STRSXP, package_count));
    for (int index = 0; index < package_count; ++index) {
        SET_STRING_ELT(r_packages, index, mkChar(packages[index].c_str()));
    }
    UNPROTECT(1);
    return r_packages;
}

SEXP r_context_get_traced_functions(SEXP r_context, SEXP r_package_name) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));

    const std::unordered_set<std::string>& function_names =
        context->get_traced_functions(package_name);
    int function_count = function_names.size();

    SEXP r_function_names = PROTECT(allocVector(STRSXP, function_count));
    int index = 0;

    for (const std::string& function_name: function_names) {
        SET_STRING_ELT(r_function_names, index, mkChar(function_name.c_str()));
        ++index;
    }
    UNPROTECT(1);
    return r_function_names;
}

SEXP r_context_is_package_traced(SEXP r_context, SEXP r_package_name) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    return ScalarLogical(context->is_package_traced(package_name));
}

SEXP r_context_is_function_traced(SEXP r_context,
                                  SEXP r_package_name,
                                  SEXP r_function_name) {
    ContextSPtr context = Context::from_sexp(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    const std::string function_name(CHAR(asChar(r_function_name)));
    return ScalarLogical(
        context->is_function_traced(package_name, function_name));
}
