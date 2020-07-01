#ifndef INSTRUMENTR_CALL_HPP
#define INSTRUMENTR_CALL_HPP

#include <string>
#include <vector>
#include "Object.hpp"

namespace instrumentr {

class Function;
class Parameter;

class Call: public Object {
  public:
    Call(std::shared_ptr<Function>& function,
         SEXP r_expression,
         SEXP r_environment,
         int frame_position)
        : Object()
        , function_(function)
        , r_expression_(r_expression)
        , r_environment_(r_environment)
        , frame_position_(frame_position)
        , active_(false)
        , r_result_(get_undefined_object()) {
        R_PreserveObject(r_expression_);
        R_PreserveObject(r_environment_);
    }

    ~Call() {
        R_ReleaseObject(r_expression_);
        R_ReleaseObject(r_environment_);
        if (is_successful()) {
            R_ReleaseObject(r_result_);
        }
    }

    std::shared_ptr<Function> get_function() const {
        return function_;
    }

    SEXP get_expression() {
        return r_expression_;
    }

    SEXP get_environment() {
        return r_environment_;
    }

    int get_frame_position() const {
        return frame_position_;
    }

    bool is_active() const {
        return active_;
    }

    void set_active() {
        active_ = true;
    }

    bool is_successful() {
        return is_defined_object(get_result());
    }

    void set_result(SEXP r_result) {
        active_ = false;
        r_result_ = r_result;
        if (is_defined_object(r_result_)) {
            R_PreserveObject(r_result_);
        }
    }

    SEXP get_result() {
        return r_result_;
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

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Call> from_sexp(SEXP r_call);

    static SEXP to_sexp(std::shared_ptr<Call> call);

    static void destroy_sexp(SEXP r_call);

  private:
    std::shared_ptr<Function> function_;
    SEXP r_expression_;
    SEXP r_environment_;
    int frame_position_;
    bool active_;
    SEXP r_result_;
    std::vector<std::shared_ptr<Parameter>> parameters_;

    static SEXP class_;
};

using CallSPtr = std::shared_ptr<Call>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CALL_HPP */
