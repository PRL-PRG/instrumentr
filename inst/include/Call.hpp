#ifndef LIGHTR_CALL_HPP
#define LIGHTR_CALL_HPP

#include <string>
#include <vector>
#include <memory>
#include "CallState.hpp"
#include <Rinternals.h>

namespace lightr {

class Argument;

using call_id_t = int;

class Call {
  public:
    Call(const std::string& package_name,
         const std::string& function_name,
         int parameter_count)
        : id_(Call::get_next_id_())
        , package_name_(package_name)
        , function_name_(function_name)
        , parameter_count_(parameter_count)
        , state_(CallState::Active) {
    }

    call_id_t get_id() const {
        return id_;
    }

    const std::string& get_package_name() const {
        return package_name_;
    }

    const std::string& get_function_name() const {
        return function_name_;
    }

    int get_parameter_count() const {
        return parameter_count_;
    }

    CallState get_state() const {
        return state_;
    }

    void set_state(CallState state) {
        state_ = state;
    }

    std::shared_ptr<Argument> get_argument(int position) const {
        return arguments_.at(position);
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Call> from_sexp(SEXP r_call);

    static SEXP to_sexp(std::shared_ptr<Call> call);

    static void destroy_sexp(SEXP r_call);

  private:
    int id_;
    std::string package_name_;
    std::string function_name_;
    int parameter_count_;
    CallState state_;
    std::vector<std::shared_ptr<Argument>> arguments_;

    static call_id_t get_next_id_();
    static call_id_t id_counter_;
    static SEXP class_;
};

using CallSPtr = std::shared_ptr<Call>;

} // namespace lightr

#endif /* LIGHTR_CALL_HPP */
