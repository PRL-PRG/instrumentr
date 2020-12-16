#include "Context.hpp"
#include "r_context.h"
#include "Application.hpp"
#include "Call.hpp"
#include "Function.hpp"
#include "Package.hpp"
#include "rdyntrace.h"

using instrumentr::Application;
using instrumentr::ApplicationAttachCallback;
using instrumentr::ApplicationAttachCallbackSPtr;
using instrumentr::ApplicationDetachCallback;
using instrumentr::ApplicationDetachCallbackSPtr;
using instrumentr::ApplicationLoadCallback;
using instrumentr::ApplicationLoadCallbackSPtr;
using instrumentr::ApplicationSPtr;
using instrumentr::ApplicationUnloadCallback;
using instrumentr::ApplicationUnloadCallbackSPtr;
using instrumentr::BuiltinCallEntryCallback;
using instrumentr::BuiltinCallEntryCallbackSPtr;
using instrumentr::BuiltinCallExitCallback;
using instrumentr::BuiltinCallExitCallbackSPtr;
using instrumentr::Call;
using instrumentr::CallEntryCallback;
using instrumentr::CallEntryCallbackSPtr;
using instrumentr::CallExitCallback;
using instrumentr::CallExitCallbackSPtr;
using instrumentr::CallSPtr;
using instrumentr::CallStack;
using instrumentr::CallStackSPtr;
using instrumentr::ClosureCallEntryCallback;
using instrumentr::ClosureCallEntryCallbackSPtr;
using instrumentr::ClosureCallExitCallback;
using instrumentr::ClosureCallExitCallbackSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::EvalEntryCallback;
using instrumentr::EvalEntryCallbackSPtr;
using instrumentr::EvalExitCallback;
using instrumentr::EvalExitCallbackSPtr;
using instrumentr::from_sexp;
using instrumentr::Function;
using instrumentr::FunctionAttachCallback;
using instrumentr::FunctionAttachCallbackSPtr;
using instrumentr::FunctionDetachCallback;
using instrumentr::FunctionDetachCallbackSPtr;
using instrumentr::FunctionSPtr;
using instrumentr::GcAllocationCallback;
using instrumentr::GcAllocationCallbackSPtr;
using instrumentr::Package;
using instrumentr::PackageAttachCallback;
using instrumentr::PackageAttachCallbackSPtr;
using instrumentr::PackageDetachCallback;
using instrumentr::PackageDetachCallbackSPtr;
using instrumentr::PackageLoadCallback;
using instrumentr::PackageLoadCallbackSPtr;
using instrumentr::PackageSPtr;
using instrumentr::PackageUnloadCallback;
using instrumentr::PackageUnloadCallbackSPtr;
using instrumentr::SpecialCallEntryCallback;
using instrumentr::SpecialCallEntryCallbackSPtr;
using instrumentr::SpecialCallExitCallback;
using instrumentr::SpecialCallExitCallbackSPtr;
using instrumentr::to_sexp;
using instrumentr::VariableAssignmentCallback;
using instrumentr::VariableAssignmentCallbackSPtr;
using instrumentr::VariableDefinitionCallback;
using instrumentr::VariableDefinitionCallbackSPtr;
using instrumentr::VariableLookupCallback;
using instrumentr::VariableLookupCallbackSPtr;
using instrumentr::VariableRemovalCallback;
using instrumentr::VariableRemovalCallbackSPtr;

SEXP r_context_create_context(SEXP r_environment) {
    ContextSPtr context = Context::create(r_environment);
    return to_sexp<Context>(context);
}

SEXP r_context_set_application_load_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationLoadCallbackSPtr callback =
        from_sexp<ApplicationLoadCallback>(r_callback);
    context->set_application_load_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_application_load_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationLoadCallbackSPtr callback =
        context->get_application_load_callback();
    return to_sexp<ApplicationLoadCallback>(callback);
}

SEXP r_context_has_application_load_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_application_load_callback());
}

SEXP r_context_set_application_unload_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationUnloadCallbackSPtr callback =
        from_sexp<ApplicationUnloadCallback>(r_callback);
    context->set_application_unload_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_application_unload_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationUnloadCallbackSPtr callback =
        context->get_application_unload_callback();
    return to_sexp<ApplicationUnloadCallback>(callback);
}

SEXP r_context_has_application_unload_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_application_unload_callback());
}

SEXP r_context_set_application_attach_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationAttachCallbackSPtr callback =
        from_sexp<ApplicationAttachCallback>(r_callback);
    context->set_application_attach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_application_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationAttachCallbackSPtr callback =
        context->get_application_attach_callback();
    return to_sexp<ApplicationAttachCallback>(callback);
}

SEXP r_context_has_application_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_application_attach_callback());
}

SEXP r_context_set_application_detach_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationDetachCallbackSPtr callback =
        from_sexp<ApplicationDetachCallback>(r_callback);
    context->set_application_detach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_application_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationDetachCallbackSPtr callback =
        context->get_application_detach_callback();
    return to_sexp<ApplicationDetachCallback>(callback);
}

SEXP r_context_has_application_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_application_detach_callback());
}

SEXP r_context_set_package_load_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageLoadCallbackSPtr callback =
        from_sexp<PackageLoadCallback>(r_callback);
    context->set_package_load_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_package_load_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageLoadCallbackSPtr callback = context->get_package_load_callback();
    return to_sexp<PackageLoadCallback>(callback);
}

SEXP r_context_has_package_load_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_package_load_callback());
}

SEXP r_context_set_package_unload_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageUnloadCallbackSPtr callback =
        from_sexp<PackageUnloadCallback>(r_callback);
    context->set_package_unload_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_package_unload_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageUnloadCallbackSPtr callback = context->get_package_unload_callback();
    return to_sexp<PackageUnloadCallback>(callback);
}

SEXP r_context_has_package_unload_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_package_unload_callback());
}

SEXP r_context_set_package_attach_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageAttachCallbackSPtr callback =
        from_sexp<PackageAttachCallback>(r_callback);
    context->set_package_attach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_package_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageAttachCallbackSPtr callback = context->get_package_attach_callback();
    return to_sexp<PackageAttachCallback>(callback);
}

SEXP r_context_has_package_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_package_attach_callback());
}

SEXP r_context_set_package_detach_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageDetachCallbackSPtr callback =
        from_sexp<PackageDetachCallback>(r_callback);
    context->set_package_detach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_package_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageDetachCallbackSPtr callback = context->get_package_detach_callback();
    return to_sexp<PackageDetachCallback>(callback);
}

SEXP r_context_has_package_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_package_detach_callback());
}

SEXP r_context_set_function_attach_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    FunctionAttachCallbackSPtr callback =
        from_sexp<FunctionAttachCallback>(r_callback);
    context->set_function_attach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_function_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    FunctionAttachCallbackSPtr callback =
        context->get_function_attach_callback();
    return to_sexp<FunctionAttachCallback>(callback);
}

SEXP r_context_has_function_attach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_function_attach_callback());
}

SEXP r_context_set_function_detach_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    FunctionDetachCallbackSPtr callback =
        from_sexp<FunctionDetachCallback>(r_callback);
    context->set_function_detach_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_function_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    FunctionDetachCallbackSPtr callback =
        context->get_function_detach_callback();
    return to_sexp<FunctionDetachCallback>(callback);
}

SEXP r_context_has_function_detach_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_function_detach_callback());
}

SEXP r_context_set_call_entry_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    CallEntryCallbackSPtr callback = from_sexp<CallEntryCallback>(r_callback);
    context->set_call_entry_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    CallEntryCallbackSPtr callback = context->get_call_entry_callback();
    return to_sexp<CallEntryCallback>(callback);
}

SEXP r_context_has_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_call_entry_callback());
}

SEXP r_context_set_call_exit_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    CallExitCallbackSPtr callback = from_sexp<CallExitCallback>(r_callback);
    context->set_call_exit_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    CallExitCallbackSPtr callback = context->get_call_exit_callback();
    return to_sexp<CallExitCallback>(callback);
}

SEXP r_context_has_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_call_exit_callback());
}

SEXP r_context_set_builtin_call_entry_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    BuiltinCallEntryCallbackSPtr callback =
        from_sexp<BuiltinCallEntryCallback>(r_callback);
    context->set_builtin_call_entry_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_builtin_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    BuiltinCallEntryCallbackSPtr callback =
        context->get_builtin_call_entry_callback();
    return to_sexp<BuiltinCallEntryCallback>(callback);
}

SEXP r_context_has_builtin_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_builtin_call_entry_callback());
}

SEXP r_context_set_builtin_call_exit_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    BuiltinCallExitCallbackSPtr callback =
        from_sexp<BuiltinCallExitCallback>(r_callback);
    context->set_builtin_call_exit_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_builtin_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    BuiltinCallExitCallbackSPtr callback =
        context->get_builtin_call_exit_callback();
    return to_sexp<BuiltinCallExitCallback>(callback);
}

SEXP r_context_has_builtin_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_builtin_call_exit_callback());
}

SEXP r_context_set_special_call_entry_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    SpecialCallEntryCallbackSPtr callback =
        from_sexp<SpecialCallEntryCallback>(r_callback);
    context->set_special_call_entry_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_special_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    SpecialCallEntryCallbackSPtr callback =
        context->get_special_call_entry_callback();
    return to_sexp<SpecialCallEntryCallback>(callback);
}

SEXP r_context_has_special_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_special_call_entry_callback());
}

SEXP r_context_set_special_call_exit_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    SpecialCallExitCallbackSPtr callback =
        from_sexp<SpecialCallExitCallback>(r_callback);
    context->set_special_call_exit_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_special_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    SpecialCallExitCallbackSPtr callback =
        context->get_special_call_exit_callback();
    return to_sexp<SpecialCallExitCallback>(callback);
}

SEXP r_context_has_special_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_special_call_exit_callback());
}

SEXP r_context_set_closure_call_entry_callback(SEXP r_context,
                                               SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ClosureCallEntryCallbackSPtr callback =
        from_sexp<ClosureCallEntryCallback>(r_callback);
    context->set_closure_call_entry_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_closure_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ClosureCallEntryCallbackSPtr callback =
        context->get_closure_call_entry_callback();
    return to_sexp<ClosureCallEntryCallback>(callback);
}

SEXP r_context_has_closure_call_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_closure_call_entry_callback());
}

SEXP r_context_set_closure_call_exit_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ClosureCallExitCallbackSPtr callback =
        from_sexp<ClosureCallExitCallback>(r_callback);
    context->set_closure_call_exit_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_closure_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ClosureCallExitCallbackSPtr callback =
        context->get_closure_call_exit_callback();
    return to_sexp<ClosureCallExitCallback>(callback);
}

SEXP r_context_has_closure_call_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_closure_call_exit_callback());
}

SEXP r_context_set_eval_entry_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    EvalEntryCallbackSPtr callback = from_sexp<EvalEntryCallback>(r_callback);
    context->set_eval_entry_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_eval_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    EvalEntryCallbackSPtr callback = context->get_eval_entry_callback();
    return to_sexp<EvalEntryCallback>(callback);
}

SEXP r_context_has_eval_entry_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_eval_entry_callback());
}

SEXP r_context_set_eval_exit_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    EvalExitCallbackSPtr callback = from_sexp<EvalExitCallback>(r_callback);
    context->set_eval_exit_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_eval_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    EvalExitCallbackSPtr callback = context->get_eval_exit_callback();
    return to_sexp<EvalExitCallback>(callback);
}

SEXP r_context_has_eval_exit_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_eval_exit_callback());
}

SEXP r_context_set_gc_allocation_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    GcAllocationCallbackSPtr callback =
        from_sexp<GcAllocationCallback>(r_callback);
    context->set_gc_allocation_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_gc_allocation_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    GcAllocationCallbackSPtr callback = context->get_gc_allocation_callback();
    return to_sexp<GcAllocationCallback>(callback);
}

SEXP r_context_has_gc_allocation_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_gc_allocation_callback());
}

SEXP r_context_set_variable_definition_callback(SEXP r_context,
                                                SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableDefinitionCallbackSPtr callback =
        from_sexp<VariableDefinitionCallback>(r_callback);
    context->set_variable_definition_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_variable_definition_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableDefinitionCallbackSPtr callback =
        context->get_variable_definition_callback();
    return to_sexp<VariableDefinitionCallback>(callback);
}

SEXP r_context_has_variable_definition_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_variable_definition_callback());
}

SEXP r_context_set_variable_assignment_callback(SEXP r_context,
                                                SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableAssignmentCallbackSPtr callback =
        from_sexp<VariableAssignmentCallback>(r_callback);
    context->set_variable_assignment_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_variable_assignment_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableAssignmentCallbackSPtr callback =
        context->get_variable_assignment_callback();
    return to_sexp<VariableAssignmentCallback>(callback);
}

SEXP r_context_has_variable_assignment_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_variable_assignment_callback());
}

SEXP r_context_set_variable_removal_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableRemovalCallbackSPtr callback =
        from_sexp<VariableRemovalCallback>(r_callback);
    context->set_variable_removal_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_variable_removal_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableRemovalCallbackSPtr callback =
        context->get_variable_removal_callback();
    return to_sexp<VariableRemovalCallback>(callback);
}

SEXP r_context_has_variable_removal_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_variable_removal_callback());
}

SEXP r_context_set_variable_lookup_callback(SEXP r_context, SEXP r_callback) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableLookupCallbackSPtr callback =
        from_sexp<VariableLookupCallback>(r_callback);
    context->set_variable_lookup_callback(callback);
    return R_NilValue;
}

SEXP r_context_get_variable_lookup_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    VariableLookupCallbackSPtr callback =
        context->get_variable_lookup_callback();
    return to_sexp<VariableLookupCallback>(callback);
}

SEXP r_context_has_variable_lookup_callback(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->has_variable_lookup_callback());
}

SEXP r_context_set_environment(SEXP r_context, SEXP r_environment) {
    ContextSPtr context = from_sexp<Context>(r_context);
    context->set_environment(r_environment);
    return R_NilValue;
}

SEXP r_context_get_environment(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return context->get_environment();
}

SEXP r_context_trace_package(SEXP r_context, SEXP r_package_name) {
    ContextSPtr context = from_sexp<Context>(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    context->trace_package(package_name);
    return R_NilValue;
}

SEXP r_context_trace_function(SEXP r_context,
                              SEXP r_package_name,
                              SEXP r_function_name) {
    ContextSPtr context = from_sexp<Context>(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    const std::string function_name(CHAR(asChar(r_function_name)));
    context->trace_function(package_name, function_name);
    return R_NilValue;
}

SEXP r_context_get_traced_packages(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
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
    ContextSPtr context = from_sexp<Context>(r_context);
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
    ContextSPtr context = from_sexp<Context>(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    return ScalarLogical(context->is_package_traced(package_name));
}

SEXP r_context_is_function_traced(SEXP r_context,
                                  SEXP r_package_name,
                                  SEXP r_function_name) {
    ContextSPtr context = from_sexp<Context>(r_context);
    const std::string package_name(CHAR(asChar(r_package_name)));
    const std::string function_name(CHAR(asChar(r_function_name)));
    return ScalarLogical(
        context->is_function_traced(package_name, function_name));
}

SEXP r_context_is_tracing_enabled(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    return ScalarLogical(context->is_tracing_enabled());
}

SEXP r_context_disable_tracing(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    context->disable_tracing();
    return R_NilValue;
}

SEXP r_context_enable_tracing(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    context->enable_tracing();
    return R_NilValue;
}

SEXP r_context_reinstate_tracing(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    context->reinstate_tracing();
    return R_NilValue;
}

SEXP r_context_get_current_callback_type(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    instrumentr::Callback::Type callback_type =
        context->get_current_callback_type();
    std::string callback_type_string = callback_type_to_string(callback_type);
    return mkString(callback_type_string.c_str());
}

SEXP r_context_initialize_tracing(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    context->initialize_tracing();
    instrumentr::add_context(context);
    return R_NilValue;
}

SEXP r_context_finalize_tracing(SEXP r_context) {
    ContextSPtr context = from_sexp<Context>(r_context);
    instrumentr::remove_context(context);
    context->finalize_tracing();
    return R_NilValue;
}

SEXP r_context_trace_code(SEXP r_context, SEXP r_code, SEXP r_environment) {
    ContextSPtr context = from_sexp<Context>(r_context);

    context->enable_tracing();
    context->push_callback_type(instrumentr::Callback::Type::Application);

#ifdef USING_DYNTRACE
    dyntrace_result_t result =
        dyntrace_trace_code(context->get_dyntracer(), r_code, r_environment);
    SEXP r_result = result.value;
#else
    SEXP r_result = Rf_eval(r_code, r_environment);
#endif /* USING_DYNTRACE */

    context->pop_callback_type();
    context->reinstate_tracing();

    return r_result;
}

SEXP r_context_trace_application_load(SEXP r_context, SEXP r_application) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationSPtr application = from_sexp<Application>(r_application);

    context->set_application(application);

    if (context->has_application_load_callback()) {
        ApplicationLoadCallbackSPtr callback =
            context->get_application_load_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_application_unload(SEXP r_context, SEXP r_application) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (context->has_application_unload_callback()) {
        ApplicationUnloadCallbackSPtr callback =
            context->get_application_unload_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application);

            context->finalize_callback_invocation();
        }
    }

    context->free_application();

    return R_NilValue;
}

SEXP r_context_trace_application_attach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (context->has_application_attach_callback()) {
        ApplicationAttachCallbackSPtr callback =
            context->get_application_attach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_application_detach(SEXP r_context, SEXP r_application) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (context->has_application_detach_callback()) {
        ApplicationDetachCallbackSPtr callback =
            context->get_application_detach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_package_load(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_package) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationSPtr application = from_sexp<Application>(r_application);
    PackageSPtr package = from_sexp<Package>(r_package);

    application->add_package(package);

    if (context->has_package_load_callback()) {
        PackageLoadCallbackSPtr callback = context->get_package_load_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_package_unload(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationSPtr application = from_sexp<Application>(r_application);
    PackageSPtr package = from_sexp<Package>(r_package);

    if (context->has_package_unload_callback()) {
        PackageUnloadCallbackSPtr callback =
            context->get_package_unload_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package);

            context->finalize_callback_invocation();
        }
    }

    application->remove_package(package);

    return R_NilValue;
}

SEXP r_context_trace_package_attach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (context->has_package_attach_callback()) {
        PackageAttachCallbackSPtr callback =
            context->get_package_attach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_package_detach(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (context->has_package_detach_callback()) {
        PackageDetachCallbackSPtr callback =
            context->get_package_detach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_function_attach(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_package,
                                     SEXP r_function) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageSPtr package = from_sexp<Package>(r_package);
    FunctionSPtr function = from_sexp<Function>(r_function);

    package->add_function(function);

    if (context->has_function_attach_callback()) {
        FunctionAttachCallbackSPtr callback =
            context->get_function_attach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package, r_function);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_function_detach(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_package,
                                     SEXP r_function) {
    ContextSPtr context = from_sexp<Context>(r_context);
    PackageSPtr package = from_sexp<Package>(r_package);
    FunctionSPtr function = from_sexp<Function>(r_function);

    if (context->has_function_detach_callback()) {
        FunctionDetachCallbackSPtr callback =
            context->get_function_detach_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context, r_application, r_package, r_function);

            context->finalize_callback_invocation();
        }
    }

    package->remove_function(function);

    return R_NilValue;
}

SEXP r_context_trace_call_entry(SEXP r_context,
                                SEXP r_application,
                                SEXP r_package,
                                SEXP r_function,
                                SEXP r_call) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationSPtr application = from_sexp<Application>(r_application);
    CallStackSPtr call_stack = application->get_call_stack();
    CallSPtr call = from_sexp<Call>(r_call);

    call->set_active();

    call_stack->push_frame(call);

    if (context->has_call_entry_callback()) {
        CallEntryCallbackSPtr callback = context->get_call_entry_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(
                r_context, r_application, r_package, r_function, r_call);

            context->finalize_callback_invocation();
        }
    }

    return R_NilValue;
}

SEXP r_context_trace_call_exit(SEXP r_context,
                               SEXP r_application,
                               SEXP r_package,
                               SEXP r_function,
                               SEXP r_result) {
    ContextSPtr context = from_sexp<Context>(r_context);
    ApplicationSPtr application = from_sexp<Application>(r_application);
    PackageSPtr package = from_sexp<Package>(r_package);
    FunctionSPtr function = from_sexp<Function>(r_function);
    CallStackSPtr call_stack = application->get_call_stack();
    CallSPtr call = call_stack->peek_frame(0);

    if (call->get_function()->get_name() != function->get_name()) {
        Rf_error("Error: unmatched call entry and exit, " //
                 "exiting '%s::%s' but expected to exit '%s'.",
                 package->get_name().c_str(),
                 function->get_name().c_str(),
                 call->get_function()->get_name().c_str());
    }

    call->set_result(r_result);

    if (context->has_call_exit_callback()) {
        CallExitCallbackSPtr callback = context->get_call_exit_callback();

        if (callback->is_active()) {
            context->initialize_callback_invocation(callback);

            callback->invoke(r_context,
                             r_application,
                             r_package,
                             r_function,
                             to_sexp<Call>(call));

            context->finalize_callback_invocation();
        }
    }

    call_stack->pop_frame();

    return R_NilValue;
}
