#ifndef INSTRUMENTR_CONTEXT_HPP
#define INSTRUMENTR_CONTEXT_HPP

#include "Object.hpp"
#include "Application.hpp"
#include "ApplicationLoadCallback.hpp"
#include "ApplicationUnloadCallback.hpp"
#include "ApplicationAttachCallback.hpp"
#include "ApplicationDetachCallback.hpp"
#include "PackageLoadCallback.hpp"
#include "PackageUnloadCallback.hpp"
#include "PackageAttachCallback.hpp"
#include "PackageDetachCallback.hpp"
#include "FunctionAttachCallback.hpp"
#include "FunctionDetachCallback.hpp"
#include "CallEntryCallback.hpp"
#include "CallExitCallback.hpp"
#include "GcAllocationCallback.hpp"
#include "GcUnmarkCallback.hpp"
#include "VariableDefinitionCallback.hpp"
#include "VariableAssignmentCallback.hpp"
#include "VariableRemovalCallback.hpp"
#include "VariableLookupCallback.hpp"
#include "BuiltinCallEntryCallback.hpp"
#include "SpecialCallEntryCallback.hpp"
#include "BuiltinCallExitCallback.hpp"
#include "SpecialCallExitCallback.hpp"
#include "ClosureCallEntryCallback.hpp"
#include "ClosureCallExitCallback.hpp"
#include "EvalEntryCallback.hpp"
#include "EvalExitCallback.hpp"
#include "ObjectCoerceCallback.hpp"
#include "ObjectDuplicateCallback.hpp"
#include "VectorCopyCallback.hpp"
#include "MatrixCopyCallback.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Rincludes.h"
#include "rdyntrace.h"

namespace instrumentr {

class Context: public Object {
  public:
    ~Context() {
        R_ReleaseObject(r_environment_);

#ifdef USING_DYNTRACE
        rdyntrace_destroy_dyntracer(dyntracer_);
        dyntracer_ = nullptr;
#endif /* USING_DYNTRACE */
    }

#define INSTRUMENTR_GENERATE_CALLBACK_GETTER(CALLBACK_CLASS,    \
                                             CALLBACK_VARIABLE) \
    CALLBACK_CLASS##SPtr get_##CALLBACK_VARIABLE() {            \
        return CALLBACK_VARIABLE##_;                            \
    }

#define INSTRUMENTR_GENERATE_CALLBACK_SETTER(CALLBACK_CLASS,      \
                                             CALLBACK_VARIABLE)   \
    void set_##CALLBACK_VARIABLE(CALLBACK_CLASS##SPtr callback) { \
        set_callback_(CALLBACK_VARIABLE##_, callback);            \
    }

#define INSTRUMENTR_GENERATE_CALLBACK_PREDICATE(CALLBACK_CLASS,    \
                                                CALLBACK_VARIABLE) \
    bool has_##CALLBACK_VARIABLE() const {                         \
        return (bool) ( CALLBACK_VARIABLE##_ );                    \
    }

#define INSTRUMENTR_GENERATE_CALLBACK_API(CALLBACK_CLASS, CALLBACK_VARIABLE) \
    INSTRUMENTR_GENERATE_CALLBACK_GETTER(CALLBACK_CLASS, CALLBACK_VARIABLE)  \
    INSTRUMENTR_GENERATE_CALLBACK_SETTER(CALLBACK_CLASS, CALLBACK_VARIABLE)  \
    INSTRUMENTR_GENERATE_CALLBACK_PREDICATE(CALLBACK_CLASS, CALLBACK_VARIABLE)


    INSTRUMENTR_GENERATE_CALLBACK_API(ApplicationLoadCallback,
                                      application_load_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ApplicationUnloadCallback,
                                      application_unload_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ApplicationAttachCallback,
                                      application_attach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ApplicationDetachCallback,
                                      application_detach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(PackageLoadCallback,
                                      package_load_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(PackageUnloadCallback,
                                      package_unload_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(PackageAttachCallback,
                                      package_attach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(PackageDetachCallback,
                                      package_detach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(FunctionAttachCallback,
                                      function_attach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(FunctionDetachCallback,
                                      function_detach_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(CallEntryCallback, call_entry_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(CallExitCallback, call_exit_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ObjectCoerceCallback,
                                      object_coerce_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ObjectDuplicateCallback,
                                      object_duplicate_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(VectorCopyCallback, vector_copy_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(MatrixCopyCallback, matrix_copy_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(BuiltinCallEntryCallback,
                                      builtin_call_entry_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(BuiltinCallExitCallback,
                                      builtin_call_exit_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(SpecialCallEntryCallback,
                                      special_call_entry_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(SpecialCallExitCallback,
                                      special_call_exit_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ClosureCallEntryCallback,
                                      closure_call_entry_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(ClosureCallExitCallback,
                                      closure_call_exit_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(EvalEntryCallback, eval_entry_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(EvalExitCallback, eval_exit_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(GcAllocationCallback,
                                      gc_allocation_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(GcUnmarkCallback, gc_unmark_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(VariableDefinitionCallback,
                                      variable_definition_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(VariableAssignmentCallback,
                                      variable_assignment_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(VariableRemovalCallback,
                                      variable_removal_callback)
    INSTRUMENTR_GENERATE_CALLBACK_API(VariableLookupCallback,
                                      variable_lookup_callback)

    void set_environment(SEXP r_environment) {
        R_ReleaseObject(r_environment_);
        r_environment_ = r_environment;
        R_PreserveObject(r_environment_);
    }

    SEXP get_environment() {
        return r_environment_;
    }

#ifdef USING_DYNTRACE
    dyntracer_t* get_dyntracer() {
        return dyntracer_;
    }
#endif /* USING_DYNTRACE */

    void set_application(ApplicationSPtr application) {
        application_ = application;
    }

    void free_application() {
        application_ = nullptr;
    }

    ApplicationSPtr get_application() const {
        return application_;
    }

    void trace_package(const std::string& package_name) {
        /* empty function list implies trace all functions of that package  */
        packages_[package_name] = std::unordered_set<std::string>();
    }

    void trace_function(const std::string& package_name,
                        const std::string& function_name) {
        auto iter = packages_.find(package_name);

        /* package has not been added before */
        if (iter == packages_.end()) {
            std::unordered_set<std::string> names;
            names.insert(function_name);
            packages_[package_name] = names;
        }
        /* package has been added with specific function names  */
        else if (iter->second.size() != 0) {
            iter->second.insert(function_name);
        }
    }

    std::vector<std::string> get_traced_packages() const {
        std::vector<std::string> keys;

        for (const auto& node: packages_) {
            keys.push_back(node.first);
        }

        return keys;
    }

    const std::unordered_set<std::string>&
    get_traced_functions(const std::string& package_name) const {
        return packages_.find(package_name)->second;
    }

    bool is_package_traced(const std::string& package_name) const {
        return packages_.find(package_name) != packages_.end();
    }

    bool is_function_traced(const std::string& package_name,
                            const std::string& function_name) const {
        auto iter = packages_.find(package_name);

        if (iter == packages_.end()) {
            return false;
        }

        const std::unordered_set<std::string>& function_names(iter->second);

        return function_names.find(function_name) != function_names.end();
    }

    void initialize_tracing() {
        push_callback_type(Callback::Type::Instrumentr);
    }

    void finalize_tracing() {
        callback_type_stack_.clear();
        tracing_status_stack_.clear();
    }

    bool is_tracing_enabled() const {
        return !tracing_status_stack_.empty() && tracing_status_stack_.back();
    }

    void set_tracing_status(bool tracing_status) {
        tracing_status_stack_.push_back(tracing_status);
    }

    void enable_tracing() {
        set_tracing_status(true);
        dyntrace_enable();
    }

    void disable_tracing() {
        set_tracing_status(false);
        dyntrace_disable();
    }

    void reinstate_tracing() {
        if (!tracing_status_stack_.empty()) {
            tracing_status_stack_.pop_back();
            bool current_status =
                !tracing_status_stack_.empty() && tracing_status_stack_.back();
            if (current_status) {
                dyntrace_enable();
            } else {
                dyntrace_disable();
            }
        }
    }

    void initialize_callback_invocation(CallbackSPtr callback) {
        disable_tracing();
        push_callback_type(callback->get_type());
    }

    void finalize_callback_invocation() {
        pop_callback_type();
        reinstate_tracing();
    }

    Callback::Type get_current_callback_type() const {
        return callback_type_stack_.back();
    }

    void push_callback_type(Callback::Type callback_type) {
        callback_type_stack_.push_back(callback_type);
    }

    Callback::Type pop_callback_type() {
        Callback::Type callback_type = callback_type_stack_.back();
        callback_type_stack_.pop_back();
        return callback_type;
    }

    static std::shared_ptr<Context> create(SEXP r_environment);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    ApplicationLoadCallbackSPtr application_load_callback_;
    ApplicationUnloadCallbackSPtr application_unload_callback_;
    ApplicationAttachCallbackSPtr application_attach_callback_;
    ApplicationDetachCallbackSPtr application_detach_callback_;
    PackageLoadCallbackSPtr package_load_callback_;
    PackageUnloadCallbackSPtr package_unload_callback_;
    PackageAttachCallbackSPtr package_attach_callback_;
    PackageDetachCallbackSPtr package_detach_callback_;
    FunctionAttachCallbackSPtr function_attach_callback_;
    FunctionDetachCallbackSPtr function_detach_callback_;
    CallEntryCallbackSPtr call_entry_callback_;
    CallExitCallbackSPtr call_exit_callback_;
    ObjectCoerceCallbackSPtr object_coerce_callback_;
    ObjectDuplicateCallbackSPtr object_duplicate_callback_;
    VectorCopyCallbackSPtr vector_copy_callback_;
    MatrixCopyCallbackSPtr matrix_copy_callback_;
    BuiltinCallEntryCallbackSPtr builtin_call_entry_callback_;
    BuiltinCallExitCallbackSPtr builtin_call_exit_callback_;
    SpecialCallEntryCallbackSPtr special_call_entry_callback_;
    SpecialCallExitCallbackSPtr special_call_exit_callback_;
    ClosureCallEntryCallbackSPtr closure_call_entry_callback_;
    ClosureCallExitCallbackSPtr closure_call_exit_callback_;
    EvalEntryCallbackSPtr eval_entry_callback_;
    EvalExitCallbackSPtr eval_exit_callback_;
    GcAllocationCallbackSPtr gc_allocation_callback_;
    GcUnmarkCallbackSPtr gc_unmark_callback_;
    VariableDefinitionCallbackSPtr variable_definition_callback_;
    VariableAssignmentCallbackSPtr variable_assignment_callback_;
    VariableRemovalCallbackSPtr variable_removal_callback_;
    VariableLookupCallbackSPtr variable_lookup_callback_;
    ApplicationSPtr application_;
    SEXP r_environment_;
    std::unordered_map<std::string, std::unordered_set<std::string>> packages_;
    std::vector<bool> tracing_status_stack_;
    std::vector<Callback::Type> callback_type_stack_;

#ifdef USING_DYNTRACE
    dyntracer_t* dyntracer_;
#endif /* USING_DYNTRACE */

    static SEXP class_;

    template <typename T>
    void set_callback_(T& left, T& right) {
        if (left) {
            left->unbind(get_environment());
        }
        left = right;
        left->bind(get_environment());
    }

#ifdef USING_DYNTRACE
    explicit Context(SEXP r_environment, dyntracer_t* dyntracer)
        : Object(), r_environment_(r_environment), dyntracer_(dyntracer) {
        R_PreserveObject(r_environment_);
    }
#else  /* USING_DYNTRACE  */
    explicit Context(SEXP r_environment)
        : Object(), r_environment_(r_environment) {
        R_PreserveObject(r_environment_);
    }
#endif /* USING_DYNTRACE  */
};     // namespace instrumentr

using ContextSPtr = std::shared_ptr<Context>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_HPP */
