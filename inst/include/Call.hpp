#ifndef LIGHTR_CALL_HPP
#define LIGHTR_CALL_HPP

#include <string>
#include <vector>
#include "CallState.hpp"
#include "Object.hpp"

namespace lightr {

class Function;
class Parameter;

class Call: public Object {
  public:
    Call(std::shared_ptr<Function> function)
        : Object(), function_(function), state_(CallState::Active) {
    }

    const std::shared_ptr<Function> get_function() const {
        return function_;
    }

    CallState get_state() const {
        return state_;
    }

    void set_state(CallState state) {
        state_ = state;
    }

    const std::vector<std::shared_ptr<Parameter>>& get_parameters() const {
        return parameters_;
    }

    std::vector<std::shared_ptr<Parameter>>& get_parameters() {
        return parameters_;
    }

    void add_parameter(std::shared_ptr<Parameter> parameter) {
        parameters_.push_back(parameter);
    }

    void set_environment(SEXP r_environment_obj) {
        r_environment_obj_ = r_environment_obj;
    }

    SEXP get_environment() {
        return r_environment_obj_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Call> from_sexp(SEXP r_call);

    static SEXP to_sexp(std::shared_ptr<Call> call);

    static void destroy_sexp(SEXP r_call);

  private:
    std::shared_ptr<Function> function_;
    CallState state_;
    std::vector<std::shared_ptr<Parameter>> parameters_;
    SEXP r_environment_obj_;

    static SEXP class_;
};

using CallSPtr = std::shared_ptr<Call>;

} // namespace lightr

#endif /* LIGHTR_CALL_HPP */
