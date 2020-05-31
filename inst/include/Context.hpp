#ifndef LIGHTR_CONTEXT_HPP
#define LIGHTR_CONTEXT_HPP

#include "Object.hpp"

namespace lightr {

class Context: public Object {
  public:
    Context()
        : Object()
        , r_initializer_(nullptr)
        , r_finalizer_(nullptr)
        , r_package_entry_callback_(nullptr)
        , r_package_exit_callback_(nullptr)
        , r_call_entry_callback_(nullptr)
        , r_call_exit_callback_(nullptr)
        , r_environment_(R_GlobalEnv) {
    }

    void set_initializer(SEXP r_initializer) {
        r_initializer_ = r_initializer;
    }

    SEXP get_initializer() {
        return r_initializer_;
    }

    bool has_initializer() const {
        return r_initializer_ != nullptr;
    }

    void set_finalizer(SEXP r_finalizer) {
        r_finalizer_ = r_finalizer;
    }

    SEXP get_finalizer() {
        return r_finalizer_;
    }

    bool has_finalizer() const {
        return r_finalizer_ != nullptr;
    }

    void set_package_entry_callback(SEXP r_package_entry_callback) {
        r_package_entry_callback_ = r_package_entry_callback;
    }

    SEXP get_package_entry_callback() {
        return r_package_entry_callback_;
    }

    bool has_package_entry_callback() const {
        return r_package_entry_callback_ != nullptr;
    }

    void set_package_exit_callback(SEXP r_package_exit_callback) {
        r_package_exit_callback_ = r_package_exit_callback;
    }

    SEXP get_package_exit_callback() {
        return r_package_exit_callback_;
    }

    bool has_package_exit_callback() const {
        return r_package_exit_callback_ != nullptr;
    }

    void set_call_entry_callback(SEXP r_call_entry_callback) {
        r_call_entry_callback_ = r_call_entry_callback;
    }

    SEXP get_call_entry_callback() {
        return r_call_entry_callback_;
    }

    bool has_call_entry_callback() const {
        return r_call_entry_callback_ != nullptr;
    }

    void set_call_exit_callback(SEXP r_call_exit_callback) {
        r_call_exit_callback_ = r_call_exit_callback;
    }

    SEXP get_call_exit_callback() {
        return r_call_exit_callback_;
    }

    bool has_call_exit_callback() const {
        return r_call_exit_callback_ != nullptr;
    }

    void set_environment(SEXP r_environment) {
        r_environment_ = r_environment;
    }

    SEXP get_environment() {
        return r_environment_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Context> from_sexp(SEXP r_context);

    static SEXP to_sexp(std::shared_ptr<Context> context);

    static void destroy_sexp(SEXP r_context);

  private:
    SEXP r_initializer_;
    SEXP r_finalizer_;
    SEXP r_package_entry_callback_;
    SEXP r_package_exit_callback_;
    SEXP r_call_entry_callback_;
    SEXP r_call_exit_callback_;
    SEXP r_environment_;

    static SEXP class_;
};

using ContextSPtr = std::shared_ptr<Context>;

} // namespace lightr

#endif /* LIGHTR_CONTEXT_HPP */
