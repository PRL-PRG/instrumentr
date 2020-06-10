#ifndef INSTRUMENTR_CONTEXT_HPP
#define INSTRUMENTR_CONTEXT_HPP
#include "Object.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace instrumentr {

extern SEXP ApplicationLoadCallbackSymbol;
extern SEXP ApplicationUnloadCallbackSymbol;
extern SEXP ApplicationAttachCallbackSymbol;
extern SEXP ApplicationDetachCallbackSymbol;
extern SEXP PackageLoadCallbackSymbol;
extern SEXP PackageUnloadCallbackSymbol;
extern SEXP PackageAttachCallbackSymbol;
extern SEXP PackageDetachCallbackSymbol;
extern SEXP FunctionAttachCallbackSymbol;
extern SEXP FunctionDetachCallbackSymbol;
extern SEXP CallEntryCallbackSymbol;
extern SEXP CallExitCallbackSymbol;

class Context: public Object {
  public:
    Context(SEXP r_environment): Object(), r_environment_(r_environment) {
        R_PreserveObject(r_environment_);
    }

    ~Context() {
        R_ReleaseObject(r_environment_);
    }

    void set_application_load_callback(SEXP r_application_load_callback) {
        set_callback_(ApplicationLoadCallbackSymbol,
                      r_application_load_callback);
    }

    SEXP get_application_load_callback() {
        return get_callback_(ApplicationLoadCallbackSymbol);
    }

    bool has_application_load_callback() {
        return has_callback_(ApplicationLoadCallbackSymbol);
    }

    void set_application_unload_callback(SEXP r_application_unload_callback) {
        set_callback_(ApplicationUnloadCallbackSymbol,
                      r_application_unload_callback);
    }

    SEXP get_application_unload_callback() {
        return get_callback_(ApplicationUnloadCallbackSymbol);
    }

    bool has_application_unload_callback() {
        return has_callback_(ApplicationUnloadCallbackSymbol);
    }

    void set_application_attach_callback(SEXP r_application_attach_callback) {
        set_callback_(ApplicationAttachCallbackSymbol,
                      r_application_attach_callback);
    }

    SEXP get_application_attach_callback() {
        return get_callback_(ApplicationAttachCallbackSymbol);
    }

    bool has_application_attach_callback() {
        return has_callback_(ApplicationAttachCallbackSymbol);
    }

    void set_application_detach_callback(SEXP r_application_detach_callback) {
        set_callback_(ApplicationDetachCallbackSymbol,
                      r_application_detach_callback);
    }

    SEXP get_application_detach_callback() {
        return get_callback_(ApplicationDetachCallbackSymbol);
    }

    bool has_application_detach_callback() {
        return has_callback_(ApplicationDetachCallbackSymbol);
    }

    void set_package_load_callback(SEXP r_package_load_callback) {
        set_callback_(PackageLoadCallbackSymbol, r_package_load_callback);
    }

    SEXP get_package_load_callback() {
        return get_callback_(PackageLoadCallbackSymbol);
    }

    bool has_package_load_callback() {
        return has_callback_(PackageLoadCallbackSymbol);
    }

    void set_package_unload_callback(SEXP r_package_unload_callback) {
        set_callback_(PackageUnloadCallbackSymbol, r_package_unload_callback);
    }

    SEXP get_package_unload_callback() {
        return get_callback_(PackageUnloadCallbackSymbol);
    }

    bool has_package_unload_callback() {
        return has_callback_(PackageUnloadCallbackSymbol);
    }

    void set_package_attach_callback(SEXP r_package_attach_callback) {
        set_callback_(PackageAttachCallbackSymbol, r_package_attach_callback);
    }

    SEXP get_package_attach_callback() {
        return get_callback_(PackageAttachCallbackSymbol);
    }

    bool has_package_attach_callback() {
        return has_callback_(PackageAttachCallbackSymbol);
    }

    void set_package_detach_callback(SEXP r_package_detach_callback) {
        set_callback_(PackageDetachCallbackSymbol, r_package_detach_callback);
    }

    SEXP get_package_detach_callback() {
        return get_callback_(PackageDetachCallbackSymbol);
    }

    bool has_package_detach_callback() {
        return has_callback_(PackageDetachCallbackSymbol);
    }

    void set_function_attach_callback(SEXP r_function_attach_callback) {
        set_callback_(FunctionAttachCallbackSymbol, r_function_attach_callback);
    }

    SEXP get_function_attach_callback() {
        return get_callback_(FunctionAttachCallbackSymbol);
    }

    bool has_function_attach_callback() {
        return has_callback_(FunctionAttachCallbackSymbol);
    }

    void set_function_detach_callback(SEXP r_function_detach_callback) {
        set_callback_(FunctionDetachCallbackSymbol, r_function_detach_callback);
    }

    SEXP get_function_detach_callback() {
        return get_callback_(FunctionDetachCallbackSymbol);
    }

    bool has_function_detach_callback() {
        return has_callback_(FunctionDetachCallbackSymbol);
    }

    void set_call_entry_callback(SEXP r_call_entry_callback) {
        set_callback_(CallEntryCallbackSymbol, r_call_entry_callback);
    }

    SEXP get_call_entry_callback() {
        return get_callback_(CallEntryCallbackSymbol);
    }

    bool has_call_entry_callback() {
        return has_callback_(CallEntryCallbackSymbol);
    }

    void set_call_exit_callback(SEXP r_call_exit_callback) {
        set_callback_(CallExitCallbackSymbol, r_call_exit_callback);
    }

    SEXP get_call_exit_callback() {
        return get_callback_(CallExitCallbackSymbol);
    }

    bool has_call_exit_callback() {
        return has_callback_(CallExitCallbackSymbol);
    }

    void set_environment(SEXP r_environment) {
        R_ReleaseObject(r_environment_);
        r_environment_ = r_environment;
        R_PreserveObject(r_environment_);
    }

    SEXP get_environment() {
        return r_environment_;
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

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Context> from_sexp(SEXP r_context);

    static SEXP to_sexp(std::shared_ptr<Context> context);

    static void destroy_sexp(SEXP r_context);

  private:
    void set_callback_(SEXP r_symbol, SEXP r_callback) {
        Rf_defineVar(r_symbol, r_callback, get_environment());
    }

    SEXP get_callback_(SEXP r_symbol) {
        SEXP value = Rf_findVarInFrame(get_environment(), r_symbol);
        return value == R_UnboundValue ? get_undefined_object() : value;
    }

    bool has_callback_(SEXP r_symbol) {
        return is_defined_object(get_callback_(r_symbol));
    }

    SEXP r_environment_;
    std::unordered_map<std::string, std::unordered_set<std::string>> packages_;

    static SEXP class_;
};

using ContextSPtr = std::shared_ptr<Context>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_HPP */
