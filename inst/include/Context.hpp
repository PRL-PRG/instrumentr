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
#include "VariableDefinitionCallback.hpp"
#include "VariableAssignmentCallback.hpp"
#include "VariableRemovalCallback.hpp"
#include "VariableLookupCallback.hpp"
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

    void set_application_load_callback(ApplicationLoadCallbackSPtr callback) {
        set_callback_(application_load_callback_, callback);
    }

    ApplicationLoadCallbackSPtr get_application_load_callback() {
        return application_load_callback_;
    }

    bool has_application_load_callback() const {
        return (bool) (application_load_callback_);
    }

    void
    set_application_unload_callback(ApplicationUnloadCallbackSPtr callback) {
        set_callback_(application_unload_callback_, callback);
    }

    ApplicationUnloadCallbackSPtr get_application_unload_callback() {
        return application_unload_callback_;
    }

    bool has_application_unload_callback() const {
        return (bool) (application_unload_callback_);
    }

    void
    set_application_attach_callback(ApplicationAttachCallbackSPtr callback) {
        set_callback_(application_attach_callback_, callback);
    }

    ApplicationAttachCallbackSPtr get_application_attach_callback() {
        return application_attach_callback_;
    }

    bool has_application_attach_callback() const {
        return (bool) (application_attach_callback_);
    }

    void
    set_application_detach_callback(ApplicationDetachCallbackSPtr callback) {
        set_callback_(application_detach_callback_, callback);
    }

    ApplicationDetachCallbackSPtr get_application_detach_callback() {
        return application_detach_callback_;
    }

    bool has_application_detach_callback() const {
        return (bool) (application_detach_callback_);
    }

    void set_package_load_callback(PackageLoadCallbackSPtr callback) {
        set_callback_(package_load_callback_, callback);
    }

    PackageLoadCallbackSPtr get_package_load_callback() {
        return package_load_callback_;
    }

    bool has_package_load_callback() const {
        return (bool) (package_load_callback_);
    }

    void set_package_unload_callback(PackageUnloadCallbackSPtr callback) {
        set_callback_(package_unload_callback_, callback);
    }

    PackageUnloadCallbackSPtr get_package_unload_callback() {
        return package_unload_callback_;
    }

    bool has_package_unload_callback() const {
        return (bool) (package_unload_callback_);
    }

    void set_package_attach_callback(PackageAttachCallbackSPtr callback) {
        set_callback_(package_attach_callback_, callback);
    }

    PackageAttachCallbackSPtr get_package_attach_callback() {
        return package_attach_callback_;
    }

    bool has_package_attach_callback() const {
        return (bool) (package_attach_callback_);
    }

    void set_package_detach_callback(PackageDetachCallbackSPtr callback) {
        set_callback_(package_detach_callback_, callback);
    }

    PackageDetachCallbackSPtr get_package_detach_callback() {
        return package_detach_callback_;
    }

    bool has_package_detach_callback() const {
        return (bool) (package_detach_callback_);
    }

    void set_function_attach_callback(FunctionAttachCallbackSPtr callback) {
        set_callback_(function_attach_callback_, callback);
    }

    FunctionAttachCallbackSPtr get_function_attach_callback() {
        return function_attach_callback_;
    }

    bool has_function_attach_callback() const {
        return (bool) (function_attach_callback_);
    }

    void set_function_detach_callback(FunctionDetachCallbackSPtr callback) {
        set_callback_(function_detach_callback_, callback);
    }

    FunctionDetachCallbackSPtr get_function_detach_callback() {
        return function_detach_callback_;
    }

    bool has_function_detach_callback() const {
        return (bool) (function_detach_callback_);
    }

    void set_call_entry_callback(CallEntryCallbackSPtr callback) {
        set_callback_(call_entry_callback_, callback);
    }

    CallEntryCallbackSPtr get_call_entry_callback() {
        return call_entry_callback_;
    }

    bool has_call_entry_callback() const {
        return (bool) (call_entry_callback_);
    }

    void set_call_exit_callback(CallExitCallbackSPtr callback) {
        set_callback_(call_exit_callback_, callback);
    }

    CallExitCallbackSPtr get_call_exit_callback() {
        return call_exit_callback_;
    }

    bool has_call_exit_callback() const {
        return (bool) (call_exit_callback_);
    }

    void
    set_variable_definition_callback(VariableDefinitionCallbackSPtr callback) {
        set_callback_(variable_definition_callback_, callback);
    }

    VariableDefinitionCallbackSPtr get_variable_definition_callback() {
        return variable_definition_callback_;
    }

    bool has_variable_definition_callback() const {
        return (bool) (variable_definition_callback_);
    }

    void
    set_variable_assignment_callback(VariableAssignmentCallbackSPtr callback) {
        set_callback_(variable_assignment_callback_, callback);
    }

    VariableAssignmentCallbackSPtr get_variable_assignment_callback() {
        return variable_assignment_callback_;
    }

    bool has_variable_assignment_callback() const {
        return (bool) (variable_assignment_callback_);
    }

    void set_variable_removal_callback(VariableRemovalCallbackSPtr callback) {
        set_callback_(variable_removal_callback_, callback);
    }

    VariableRemovalCallbackSPtr get_variable_removal_callback() {
        return variable_removal_callback_;
    }

    bool has_variable_removal_callback() const {
        return (bool) (variable_removal_callback_);
    }

    void set_variable_lookup_callback(VariableLookupCallbackSPtr callback) {
        set_callback_(variable_lookup_callback_, callback);
    }

    VariableLookupCallbackSPtr get_variable_lookup_callback() {
        return variable_lookup_callback_;
    }

    bool has_variable_lookup_callback() const {
        return (bool) (variable_lookup_callback_);
    }

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
    }

    void disable_tracing() {
        set_tracing_status(false);
    }

    void reinstate_tracing() {
        if (!tracing_status_stack_.empty()) {
            tracing_status_stack_.pop_back();
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
};

using ContextSPtr = std::shared_ptr<Context>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_HPP */
